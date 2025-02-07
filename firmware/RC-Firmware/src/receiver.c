#include <stdio.h>
#include <string.h>

#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"

#include "nvs_flash.h"

#define TAG "ESP-NOW-RECEIVER"

// arbitrary struct for testing
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

// struct to transfer data
/*
typedef struct struct_data {
    float input_signal;      // 0% - 100% throttle from hall sensor
    float battery_level;     // battery levels from longboard
    int rpm_reading;         // vesc rpm rating
} struct_data;
*/

struct_message my_data;

// callback when data received
void on_data_recv(const uint8_t *mac_addr, const uint8_t *incoming_data, int len) {
    memcpy(&my_data, incoming_data, sizeof(my_data));
    
    ESP_LOGI(TAG, "Data received from %02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5]);

    ESP_LOGI(TAG, "Bytes received: %d", len);
    ESP_LOGI(TAG, "Char: %s", my_data.a);
    ESP_LOGI(TAG, "Int: %d", my_data.b);
    ESP_LOGI(TAG, "Float: %.2f", my_data.c);
    ESP_LOGI(TAG, "Bool: %s", my_data.d ? "true" : "false");
}

void app_main() {
    // initializing nvs
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // wi-fi initialization in station mode
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    // esp-now initialization
    if (esp_now_init() != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing ESP-NOW");
        return;
    }

    // callback upon successful reception
    esp_now_register_recv_cb(on_data_recv);

    ESP_LOGI(TAG, "ESP-NOW receiver initialized");
}