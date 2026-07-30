#include "iic.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include <stdint.h>

static const char *TAG = "IIC";

static i2c_master_bus_handle_t s_iic_bus = NULL;
static i2c_master_dev_handle_t s_iic_dev = NULL;

esp_err_t iic_init(void)
{
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = -1,
        .sda_io_num = (gpio_num_t)IIC_SDA_IO,
        .scl_io_num = (gpio_num_t)IIC_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .trans_queue_depth = 0,
        .flags.enable_internal_pullup = true,
    };
    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &s_iic_bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "IIC总线初始化失败");
        return ret;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x00,
        .scl_speed_hz = IIC_FREQ_HZ,
    };
    ret = i2c_master_bus_add_device(s_iic_bus, &dev_cfg, &s_iic_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "IIC总线初始化失败");
        return ret;
    }
    ESP_LOGI(TAG, "IIC IO总线初始化成功");
    return ESP_OK;
}

esp_err_t iic_send_bytes(unsigned char addr, unsigned char *txd, unsigned int len)
{
    if (txd == NULL || len == 0 || s_iic_dev == NULL) {
        return ESP_ERR_INVALID_STATE;
    }
    esp_err_t ret = i2c_master_device_change_address(s_iic_dev, (uint16_t)addr, IIC_TIMEOUT_MS);
    if (ret != ESP_OK) {
        return ret;
    }
    return i2c_master_transmit(s_iic_dev, (const uint8_t *)txd, (size_t)len, IIC_TIMEOUT_MS);
}

unsigned char iic_read_byte(unsigned char addr)
{
    uint8_t byte = 0;
    if (s_iic_dev == NULL) {
        return 0;
    }
    if (i2c_master_device_change_address(s_iic_dev, (uint16_t)addr, IIC_TIMEOUT_MS) != ESP_OK) {
        return 0;
    }
    i2c_master_receive(s_iic_dev, &byte, 1, IIC_TIMEOUT_MS);
    return (unsigned char)byte;
}
