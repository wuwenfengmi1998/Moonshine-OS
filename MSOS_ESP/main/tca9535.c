#include "tca9535.h"
#include "iic.h"
#include "esp_log.h"

static const char *TAG = "TCA9535";

esp_err_t tca9535_read_reg(uint8_t reg, uint8_t *val)
{
    if (val == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    return iic_read_reg(TCA9535_ADDRESS, reg, val, 1);
}

esp_err_t tca9535_write_reg(uint8_t reg, uint8_t val)
{
    unsigned char buf[2] = {reg, val};
    return iic_send_bytes(TCA9535_ADDRESS, buf, 2);
}

esp_err_t tca9535_set_dir(uint16_t dir)
{
    esp_err_t ret = tca9535_write_reg(TCA9535_REG_CONFIG0, (uint8_t)(dir & 0xFF));
    if (ret != ESP_OK) {
        return ret;
    }
    return tca9535_write_reg(TCA9535_REG_CONFIG1, (uint8_t)(dir >> 8));
}

esp_err_t tca9535_set_output(uint16_t val)
{
    esp_err_t ret = tca9535_write_reg(TCA9535_REG_OUTPUT0, (uint8_t)(val & 0xFF));
    if (ret != ESP_OK) {
        return ret;
    }
    return tca9535_write_reg(TCA9535_REG_OUTPUT1, (uint8_t)(val >> 8));
}

esp_err_t tca9535_get_output(uint16_t *val)
{
    if (val == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t p0 = 0, p1 = 0;
    esp_err_t ret = tca9535_read_reg(TCA9535_REG_OUTPUT0, &p0);
    if (ret != ESP_OK) {
        return ret;
    }
    ret = tca9535_read_reg(TCA9535_REG_OUTPUT1, &p1);
    if (ret != ESP_OK) {
        return ret;
    }
    *val = (uint16_t)p0 | ((uint16_t)p1 << 8);
    return ESP_OK;
}

esp_err_t tca9535_get_input(uint16_t *val)
{
    if (val == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t p0 = 0, p1 = 0;
    esp_err_t ret = tca9535_read_reg(TCA9535_REG_INPUT0, &p0);
    if (ret != ESP_OK) {
        return ret;
    }
    ret = tca9535_read_reg(TCA9535_REG_INPUT1, &p1);
    if (ret != ESP_OK) {
        return ret;
    }
    *val = (uint16_t)p0 | ((uint16_t)p1 << 8);
    return ESP_OK;
}

esp_err_t tca9535_pin_mode(uint8_t pin, uint8_t mode)
{
    if (pin >= TCA9535_PIN_COUNT) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t reg = (pin < 8) ? TCA9535_REG_CONFIG0 : TCA9535_REG_CONFIG1;
    uint8_t bit = pin & 0x07;
    uint8_t cur = 0;
    esp_err_t ret = tca9535_read_reg(reg, &cur);
    if (ret != ESP_OK) {
        return ret;
    }
    if (mode == TCA9535_INPUT) {
        cur |= (1 << bit);
    } else {
        cur &= ~(1 << bit);
    }
    return tca9535_write_reg(reg, cur);
}

esp_err_t tca9535_digital_write(uint8_t pin, uint8_t val)
{
    if (pin >= TCA9535_PIN_COUNT) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t reg = (pin < 8) ? TCA9535_REG_OUTPUT0 : TCA9535_REG_OUTPUT1;
    uint8_t bit = pin & 0x07;
    uint8_t cur = 0;
    esp_err_t ret = tca9535_read_reg(reg, &cur);
    if (ret != ESP_OK) {
        return ret;
    }
    if (val) {
        cur |= (1 << bit);
    } else {
        cur &= ~(1 << bit);
    }
    return tca9535_write_reg(reg, cur);
}

esp_err_t tca9535_digital_read(uint8_t pin, uint8_t *val)
{
    if (pin >= TCA9535_PIN_COUNT || val == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t reg = (pin < 8) ? TCA9535_REG_INPUT0 : TCA9535_REG_INPUT1;
    uint8_t bit = pin & 0x07;
    uint8_t cur = 0;
    esp_err_t ret = tca9535_read_reg(reg, &cur);
    if (ret != ESP_OK) {
        return ret;
    }
    *val = (cur >> bit) & 0x01;
    return ESP_OK;
}

esp_err_t tca9535_init(void)
{
    esp_err_t ret = tca9535_write_reg(TCA9535_REG_POLARITY0, 0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TCA9535初始化失败");
        return ret;
    }
    tca9535_write_reg(TCA9535_REG_POLARITY1, 0x00);
    tca9535_write_reg(TCA9535_REG_OUTPUT0, 0x00);
    tca9535_write_reg(TCA9535_REG_OUTPUT1, 0x00);
    ret = tca9535_set_dir(0xFFFF);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TCA9535初始化失败");
        return ret;
    }
    ESP_LOGI(TAG, "TCA9535初始化成功, 地址0x%02X", TCA9535_ADDRESS);
    return ESP_OK;
}
