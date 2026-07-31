#ifndef POWER_H_
#define POWER_H_

#include "esp_err.h"


#define POWER_EN_PIN         TCA9535_PIN_P12
#define POWER_KEY_PIN        TCA9535_PIN_P13

esp_err_t power_init(void);

#endif
