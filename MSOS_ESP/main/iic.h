#ifndef IIC_H_
#define IIC_H_

#include "esp_err.h"

#define IIC_SDA_IO          0
#define IIC_SCL_IO          1
#define IIC_FREQ_HZ         100000
#define IIC_TIMEOUT_MS      1000
#define IIC_SCAN_TIMEOUT_MS 50

esp_err_t iic_init(void);
void iic_scan(void);
esp_err_t iic_send_bytes(unsigned char addr, unsigned char *txd, unsigned int len);
unsigned char iic_read_byte(unsigned char addr);

#endif
