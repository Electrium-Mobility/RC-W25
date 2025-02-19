#include <stdio.h>
#include <string.h>
#include <pairing.h>

#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"

#include "nvs_flash.h"

struct_message my_data;

uint8_t peer_mac[] = {0xA0, 0xB7, 0x65, 0x04, 0x01, 0xA0};

void readMacAddress() {
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK)
    {
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
               baseMac[0], baseMac[1], baseMac[2],
               baseMac[3], baseMac[4], baseMac[5]);
    }
    else
    {
        ESP_LOGI(PAIRING_TAG, "Failed to read MAC address");
    }
    vTaskDelete(NULL);
}

// callback for receiving data
void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len) {
    const uint8_t *mac_addr = recv_info->src_addr;
    memcpy(&my_data, incoming_data, sizeof(my_data));
    
    ESP_LOGI(PAIRING_TAG, "Data received from %02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5]);

    ESP_LOGI(PAIRING_TAG, "Bytes received: %d", len);
    ESP_LOGI(PAIRING_TAG, "A: %s", my_data.a);
    ESP_LOGI(PAIRING_TAG, "B: %d", my_data.b);
    ESP_LOGI(PAIRING_TAG, "C: %.2f", my_data.c);
    ESP_LOGI(PAIRING_TAG, "D: %s", my_data.d ? "true" : "false");
}

// callback for sending data
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGI(PAIRING_TAG, "Send status to %02X:%02X:%02X:%02X:%02X:%02X -> %s",
             mac_addr[0], mac_addr[1], mac_addr[2],
             mac_addr[3], mac_addr[4], mac_addr[5],
             (status == ESP_NOW_SEND_SUCCESS) ? "Success" : "Fail");
}

void pair() {
    // register callbacks
    esp_now_register_send_cb(on_data_sent);
    esp_now_register_recv_cb(on_data_recv);

    // configure peer device (receiver)
    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, peer_mac, 6);
    peer_info.channel = 0;
    peer_info.encrypt = false;

    if (esp_now_add_peer(&peer_info) != ESP_OK) {
        ESP_LOGE(PAIRING_TAG, "Failed to add peer");
        return;
    }

    // prepare and transmit data
    strcpy(my_data.a, "THIS IS A CHAR");
    my_data.b = 10;
    my_data.c = 1.2;
    my_data.d = false;

    // callback upon successful transmission
    while (1) {
        esp_err_t result = esp_now_send(peer_mac, (uint8_t *)&my_data, sizeof(my_data));
        if (result == ESP_OK) {
            ESP_LOGI(PAIRING_TAG, "Data sent successfully");
        } else {
            ESP_LOGE(PAIRING_TAG, "Error sending data");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}