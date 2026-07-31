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

// P0 口 (0-7)
#define TCA9535_PIN_P00      0    // P0.0
#define TCA9535_PIN_P01      1    // P0.1
#define TCA9535_PIN_P02      2    // P0.2
#define TCA9535_PIN_P03      3    // P0.3
#define TCA9535_PIN_P04      4    // P0.4
#define TCA9535_PIN_P05      5    // P0.5
#define TCA9535_PIN_P06      6    // P0.6
#define TCA9535_PIN_P07      7    // P0.7

// P1 口 (8-15)
#define TCA9535_PIN_P10      8    // P1.0
#define TCA9535_PIN_P11      9    // P1.1
#define TCA9535_PIN_P12      10   // P1.2
#define TCA9535_PIN_P13      11   // P1.3
#define TCA9535_PIN_P14      12   // P1.4
#define TCA9535_PIN_P15      13   // P1.5
#define TCA9535_PIN_P16      14   // P1.6
#define TCA9535_PIN_P17      15   // P1.7

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
