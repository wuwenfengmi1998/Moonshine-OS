#include "power.h"
#include "button.h"
#include "tca9535.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "POWER";

#define POWER_EN_LATCH_DELAY_MS     100
#define POWER_TASK_POLL_MS          10
#define POWER_TASK_STACK            2048
#define POWER_TASK_PRIO             5

static bool s_latched = false;

static void power_latch_on(void)
{
    tca9535_digital_write(POWER_EN_PIN, 0);
    vTaskDelay(POWER_EN_LATCH_DELAY_MS / portTICK_PERIOD_MS);
    tca9535_digital_write(POWER_EN_PIN, 1);
    s_latched = true;
    ESP_LOGI(TAG, "电源自锁, POWER_EN=1");
}

static void power_off(void)
{
    ESP_LOGI(TAG, "关机, POWER_EN=0");
    tca9535_digital_write(POWER_EN_PIN, 0);
}

static void power_task(void *arg)
{
    uint8_t level = 1;
    while (1) {
        if (tca9535_digital_read(POWER_KEY_PIN, &level) != ESP_OK) {
            vTaskDelay(POWER_TASK_POLL_MS / portTICK_PERIOD_MS);
            continue;
        }
        button_updata(&button_power, level);
        if (button_longpass(&button_power)) {
            if (s_latched) {
                power_off();
            } else {
                power_latch_on();
            }
        }
        vTaskDelay(POWER_TASK_POLL_MS / portTICK_PERIOD_MS);
    }
}

esp_err_t power_init(void)
{
    esp_err_t ret = tca9535_pin_mode(POWER_EN_PIN, TCA9535_OUTPUT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "POWER_EN引脚配置失败");
        return ret;
    }
    ret = tca9535_digital_write(POWER_EN_PIN, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "POWER_EN复位失败");
        return ret;
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    // 背光测试: P1.0 先设输出再置高
    tca9535_pin_mode(TCA9535_PIN_P10, TCA9535_OUTPUT);
    tca9535_digital_write(TCA9535_PIN_P10, 1);

    ESP_LOGI(TAG, "电源管理初始化成功");
    return ESP_OK;
}
