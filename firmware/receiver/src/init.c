#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "semaphore.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "init.h"

SemaphoreHandle_t pairingMutex;

void init() {
    initEspNow();
}

void initEspNow() {
    pairingMutex = xSemaphoreCreateMutex();
    if (pairingMutex == NULL) {
        ESP_LOGE(INIT_TAG, "Failed to create mutex");
    }
    if (xSemaphoreTake(pairingMutex, portMAX_DELAY) != pdTRUE) {
        ESP_LOGE(INIT_TAG, "Failed to take mutex");
    }

    // initializing nvs
    ESP_LOGI(INIT_TAG, "Initializing ESP-NOW");
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
        ESP_LOGE(INIT_TAG, "Error initializing ESP-NOW");
        return;
    }

    xSemaphoreGive(pairingMutex);
    vTaskDelete(NULL);
}