#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "init.h"

void init() {
    //Initialize all ADC pins
    ESP_LOGI(INIT_TAG, "Configuring ADC pin");
    adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit (0-4095) B/c 2^12 = 4096

    adc1_config_channel_atten(HALL_EFFECT, ADC_ATTEN_DB_12);

    //Initialize all GPIO pins
    ESP_LOGI(INIT_TAG, "Initializing Haptic GPIO");
    gpio_pad_select_gpio(HAPTIC_CNTL);
    gpio_set_direction(HAPTIC_CNTL, GPIO_MODE_INPUT);

    ESP_LOGI(INIT_TAG, "Initializing Safe Mode GPIO");
    gpio_pad_select_gpio(SAFE_MODE);
    gpio_set_direction(SAFE_MODE, GPIO_MODE_INPUT);

    ESP_LOGI(INIT_TAG, "Initializing onboard LED");
    gpio_reset_pin(LED_PIN); 
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  
    gpio_set_level(LED_PIN, 1);  

    //Initialize ESP-NOW
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
}