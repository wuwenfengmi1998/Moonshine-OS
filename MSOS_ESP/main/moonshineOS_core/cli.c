#include "cli.h"
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "iic.h"

#define CLI_LINE_MAX    256
#define CLI_MAX_ARGS    16
#define CLI_TASK_STACK  4096
#define CLI_TASK_PRIO   5
#define CLI_UART_NUM    UART_NUM_0
#define CLI_UART_RX_BUF 256

typedef int (*cli_cmd_func_t)(int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    cli_cmd_func_t func;
} cli_cmd_t;

static int cli_cmd_help(int argc, char **argv);
static int cli_cmd_iic(int argc, char **argv);

static const cli_cmd_t s_cmds[] = {
    {"help", "help                  显示所有可用命令", cli_cmd_help},
    {"iic",  "iic scan              扫描I2C总线设备",  cli_cmd_iic},
};
static const int s_cmd_count = sizeof(s_cmds) / sizeof(s_cmds[0]);

static int cli_cmd_help(int argc, char **argv)
{
    printf("可用命令:\n");
    for (int i = 0; i < s_cmd_count; i++) {
        printf("  %s\n", s_cmds[i].help);
    }
    return 0;
}

static int cli_cmd_iic(int argc, char **argv)
{
    if (argc < 2) {
        printf("用法: iic <scan>\n");
        return 1;
    }
    if (strcmp(argv[1], "scan") == 0) {
        iic_scan();
    } else {
        printf("未知子命令: %s\n用法: iic <scan>\n", argv[1]);
        return 1;
    }
    return 0;
}

static int read_line(char *buf, int maxlen)
{
    int i = 0;
    while (i < maxlen - 1) {
        int c = getchar();
        if (c < 0) {
            clearerr(stdin);
            vTaskDelay(1);
            continue;
        }
        if (c == '\r' || c == '\n') {
            putchar('\n');
            break;
        }
        if (c == 0x08 || c == 0x7F) {
            if (i > 0) {
                i--;
                fputs("\b \b", stdout);
            }
            continue;
        }
        buf[i++] = (char)c;
        putchar(c);
    }
    buf[i] = '\0';
    return i;
}

static void dispatch(char *line)
{
    char *argv[CLI_MAX_ARGS];
    int argc = 0;
    char *tok = strtok(line, " \t");
    while (tok != NULL && argc < CLI_MAX_ARGS) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    if (argc == 0) {
        return;
    }
    for (int i = 0; i < s_cmd_count; i++) {
        if (strcmp(s_cmds[i].name, argv[0]) == 0) {
            s_cmds[i].func(argc, argv);
            return;
        }
    }
    printf("未知命令: %s (输入 help 查看可用命令)\n", argv[0]);
}

void cli_init(void)
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    uart_driver_install(CLI_UART_NUM, CLI_UART_RX_BUF, 0, 0, NULL, 0);
    uart_vfs_dev_use_driver(CLI_UART_NUM);
}

static void cli_task(void *arg)
{
    cli_run();
}

void cli_start(void)
{
    xTaskCreate(cli_task, "cli", CLI_TASK_STACK, NULL, CLI_TASK_PRIO, NULL);
}

void cli_run(void)
{
    char line[CLI_LINE_MAX];
    printf("\nMoonshineOS CLI ready. 输入 help 查看命令。\n");
    while (1) {
        printf("MSOS> ");
        read_line(line, sizeof(line));
        dispatch(line);
    }
}
