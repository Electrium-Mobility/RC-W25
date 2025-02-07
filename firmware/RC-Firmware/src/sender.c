#include <stdio.h>
#include <string.h>

#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"

#include "nvs_flash.h"

#define TAG "ESP-NOW-SENDER"

// arbitrary struct for testing
typedef struct struct_message {
    char a[32];  // string data
    int b;       // integer data
    float c;     // float data
    bool d;      // boolean flag
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

uint8_t peer_mac[] = {0x24, 0x0A, 0xC4, 0x7A, 0xDE, 0xFA}; // replace with receiver mac address

// callback when data sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "Send status to %02X:%02X:%02X:%02X:%02X:%02X -> %s",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5],
             (status == ESP_NOW_SEND_SUCCESS) ? "Success" : "Fail");
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
    esp_now_register_send_cb(on_data_sent);

    // configure peer device (receiver)
    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, broadcast_address, 6);
    peer_info.channel = 0;
    peer_info.encrypt = false;

    if (esp_now_add_peer(&peer_info) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add peer");
        return;
    }

    // prepare and transmit data
    strcpy(my_data.a, "THIS IS A CHAR");
    my_data.b = 10;
    my_data.c = 1.2;
    my_data.d = false;

    // callback upon successful transmission
    while (true) {
        esp_err_t result = esp_now_send(broadcast_address, (uint8_t *)&my_data, sizeof(my_data));
        if (result == ESP_OK) {
            ESP_LOGI(TAG, "Data sent successfully");
        } else {
            ESP_LOGE(TAG, "Error sending data");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
