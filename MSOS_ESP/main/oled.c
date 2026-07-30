#include "oled.h"
#include "iic.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void oled_cmd(unsigned char IIC_Command)
{
    unsigned char buf[2] = {0x00, IIC_Command};
    iic_send_bytes(OLED_ADDRESS, buf, 2);
}

void oled_dat(unsigned char IIC_Data)
{
    unsigned char buf[2] = {0x40, IIC_Data};
    iic_send_bytes(OLED_ADDRESS, buf, 2);
}

void oled_set_pos(unsigned char x, unsigned char y)
{
    x += OLED_COL_OFFSET;
    oled_cmd(0xb0 + 7 - y);
    oled_cmd(((x & 0xf0) >> 4) | 0x10);
    oled_cmd((x & 0x0f) | 0x00);
}

void oled_init_dram(char a)
{
    for (unsigned char y = 0; y < Y_WIDTH_; y++) {
        oled_set_pos(0, y);
        for (unsigned char x = 0; x < X_WIDTH; x++) {
            oled_dat(a);
        }
    }
}

void oled_set_lum(unsigned char a)
{
    oled_cmd(0x81);
    oled_cmd((unsigned char)(a * 255 / 100));
}

void oled_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(200));

    oled_cmd(0xAE);
    oled_cmd(0xD5);
    oled_cmd(0x80);
    oled_cmd(0xA8);
    oled_cmd(0x3F);
    oled_cmd(0xD3);
    oled_cmd(0x00);
    oled_cmd(0x40);
#if (OLED_CONTROLLER == OLED_CTRL_SH1106)
    oled_cmd(0xAD);
    oled_cmd(0x8B);
#elif (OLED_CONTROLLER == OLED_CTRL_ST7315)
    oled_cmd(0x8D);
    oled_cmd(0x14);
#endif
    oled_cmd(0x20);
    oled_cmd(0x10);
    oled_cmd(0xA1);
    oled_cmd(0xC0);
    oled_cmd(0xD9);
    oled_cmd(0xf1);
    oled_cmd(0xDB);
    oled_cmd(0x30);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);

    oled_cmd(0x81);
    oled_cmd(0xff);

    oled_init_dram(0);
}
