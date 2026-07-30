#include "state.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>

static const char *TAG = "STATE";

static SemaphoreHandle_t s_mutex = NULL;

static system_state_t s_state = {
    .battery_percent      = 0,
    .battery_valid        = false,
    .iic_device_count     = 0,
    .wifi_ssid_head       = NULL,
    .wifi_ssid_count      = 0,
    .wifi_connected_ssid  = {0},
};

static void state_lock(void)
{
    if (s_mutex != NULL) {
        xSemaphoreTake(s_mutex, portMAX_DELAY);
    }
}

static void state_unlock(void)
{
    if (s_mutex != NULL) {
        xSemaphoreGive(s_mutex);
    }
}

void state_init(void)
{
    s_mutex = xSemaphoreCreateMutex();
    if (s_mutex == NULL) {
        ESP_LOGE(TAG, "互斥锁创建失败");
    }
}

void state_set_battery(uint8_t percent)
{
    if (percent > 100) {
        percent = 100;
    }
    state_lock();
    s_state.battery_percent = percent;
    s_state.battery_valid   = true;
    state_unlock();
}

uint8_t state_get_battery(void)
{
    state_lock();
    uint8_t val = s_state.battery_percent;
    state_unlock();
    return val;
}

void state_clear_iic_devices(void)
{
    state_lock();
    s_state.iic_device_count = 0;
    state_unlock();
}

void state_add_iic_device(uint8_t addr)
{
    state_lock();
    if (s_state.iic_device_count < STATE_IIC_MAX_DEVICES) {
        s_state.iic_devices[s_state.iic_device_count] = addr;
        s_state.iic_device_count++;
    }
    state_unlock();
}

uint8_t state_get_iic_device_count(void)
{
    state_lock();
    uint8_t count = s_state.iic_device_count;
    state_unlock();
    return count;
}

uint8_t state_get_iic_device(uint8_t index)
{
    state_lock();
    uint8_t addr = 0;
    if (index < s_state.iic_device_count) {
        addr = s_state.iic_devices[index];
    }
    state_unlock();
    return addr;
}

void state_clear_wifi_ssids(void)
{
    state_lock();
    wifi_ssid_node_t *node = s_state.wifi_ssid_head;
    while (node != NULL) {
        wifi_ssid_node_t *next = node->next;
        free(node);
        node = next;
    }
    s_state.wifi_ssid_head  = NULL;
    s_state.wifi_ssid_count = 0;
    state_unlock();
}

esp_err_t state_add_wifi_ssid(const char *ssid)
{
    if (ssid == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    wifi_ssid_node_t *node = (wifi_ssid_node_t *)malloc(sizeof(wifi_ssid_node_t));
    if (node == NULL) {
        ESP_LOGE(TAG, "WiFi SSID节点分配失败");
        return ESP_ERR_NO_MEM;
    }
    strncpy(node->ssid, ssid, STATE_WIFI_SSID_MAX_LEN);
    node->ssid[STATE_WIFI_SSID_MAX_LEN] = '\0';
    node->next = NULL;

    state_lock();
    if (s_state.wifi_ssid_head == NULL) {
        s_state.wifi_ssid_head = node;
    } else {
        wifi_ssid_node_t *tail = s_state.wifi_ssid_head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = node;
    }
    s_state.wifi_ssid_count++;
    state_unlock();
    return ESP_OK;
}

uint8_t state_get_wifi_ssid_count(void)
{
    state_lock();
    uint8_t count = s_state.wifi_ssid_count;
    state_unlock();
    return count;
}

const wifi_ssid_node_t *state_wifi_ssids_lock(void)
{
    state_lock();
    return s_state.wifi_ssid_head;
}

void state_wifi_ssids_unlock(void)
{
    state_unlock();
}

void state_set_wifi_connected_ssid(const char *ssid)
{
    state_lock();
    if (ssid != NULL) {
        strncpy(s_state.wifi_connected_ssid, ssid, STATE_WIFI_SSID_MAX_LEN);
        s_state.wifi_connected_ssid[STATE_WIFI_SSID_MAX_LEN] = '\0';
    } else {
        s_state.wifi_connected_ssid[0] = '\0';
    }
    state_unlock();
}

const char *state_get_wifi_connected_ssid(void)
{
    state_lock();
    const char *ssid = s_state.wifi_connected_ssid;
    state_unlock();
    return ssid;
}
