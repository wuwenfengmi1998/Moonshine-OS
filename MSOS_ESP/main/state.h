#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

#define STATE_IIC_MAX_DEVICES   16
#define STATE_WIFI_SSID_MAX_LEN 32

typedef struct wifi_ssid_node {
    char ssid[STATE_WIFI_SSID_MAX_LEN + 1];
    struct wifi_ssid_node *next;
} wifi_ssid_node_t;

typedef struct {
    uint8_t  battery_percent;
    bool     battery_valid;

    uint8_t  iic_devices[STATE_IIC_MAX_DEVICES];
    uint8_t  iic_device_count;

    wifi_ssid_node_t *wifi_ssid_head;
    uint8_t  wifi_ssid_count;
    char     wifi_connected_ssid[STATE_WIFI_SSID_MAX_LEN + 1];
} system_state_t;

void state_init(void);

void     state_set_battery(uint8_t percent);
uint8_t  state_get_battery(void);

void     state_clear_iic_devices(void);
void     state_add_iic_device(uint8_t addr);
uint8_t  state_get_iic_device_count(void);
uint8_t  state_get_iic_device(uint8_t index);

void          state_clear_wifi_ssids(void);
esp_err_t     state_add_wifi_ssid(const char *ssid);
uint8_t       state_get_wifi_ssid_count(void);
const wifi_ssid_node_t *state_wifi_ssids_lock(void);
void          state_wifi_ssids_unlock(void);
void          state_set_wifi_connected_ssid(const char *ssid);
const char   *state_get_wifi_connected_ssid(void);

#endif
