#ifndef TCA9535_H_
#define TCA9535_H_

#include "esp_err.h"
#include <stdint.h>

#define TCA9535_ADDRESS     0x20

#define TCA9535_REG_INPUT0      0x00
#define TCA9535_REG_INPUT1      0x01
#define TCA9535_REG_OUTPUT0     0x02
#define TCA9535_REG_OUTPUT1     0x03
#define TCA9535_REG_POLARITY0   0x04
#define TCA9535_REG_POLARITY1   0x05
#define TCA9535_REG_CONFIG0     0x06
#define TCA9535_REG_CONFIG1     0x07

#define TCA9535_INPUT   1
#define TCA9535_OUTPUT  0

#define TCA9535_PIN_COUNT   16

esp_err_t tca9535_init(void);

esp_err_t tca9535_read_reg(uint8_t reg, uint8_t *val);
esp_err_t tca9535_write_reg(uint8_t reg, uint8_t val);

esp_err_t tca9535_set_dir(uint16_t dir);
esp_err_t tca9535_set_output(uint16_t val);
esp_err_t tca9535_get_output(uint16_t *val);
esp_err_t tca9535_get_input(uint16_t *val);

esp_err_t tca9535_pin_mode(uint8_t pin, uint8_t mode);
esp_err_t tca9535_digital_write(uint8_t pin, uint8_t val);
esp_err_t tca9535_digital_read(uint8_t pin, uint8_t *val);

#endif
