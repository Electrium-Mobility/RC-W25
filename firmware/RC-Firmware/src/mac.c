#include <stdio.h>

#include <esp_wifi.h>
#include <esp_log.h>
#include "esp_netif.h"

static const char *TAG = "MAC_ADDRESS";

void read_mac_address() {
    uint8_t mac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, mac);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "ESP32 MAC Address: %02x:%02x:%02x:%02x:%02x:%02x",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        ESP_LOGE(TAG, "Failed to read MAC address");
    }
}

void app_main() {
    // initialize wi-fi
    esp_netif_init();
    esp_event_loop_create_default();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    read_mac_address();
}
