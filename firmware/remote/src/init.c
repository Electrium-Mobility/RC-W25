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
#include "ssd1306.h"
#include "electriumLogo.h"

#include "init.h"

SSD1306_t dev;
SemaphoreHandle_t pairingMutex;

void init() {
    initADC();
    initGPIO();
    initEspNow();
    initDisplay();
}

void initADC() {
    ESP_LOGI(INIT_TAG, "Configuring ADC pin");
    adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit (0-4095) B/c 2^12 = 4096

    adc1_config_channel_atten(HALL_EFFECT, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(BATTERY_PERCENT, ADC_ATTEN_DB_12);
}

void initGPIO() {
    ESP_LOGI(INIT_TAG, "Initializing Haptic GPIO");
    esp_rom_gpio_pad_select_gpio(HAPTIC_CNTL);
    gpio_set_direction(HAPTIC_CNTL, GPIO_MODE_INPUT);

    ESP_LOGI(INIT_TAG, "Initializing Safe Mode GPIO");
    esp_rom_gpio_pad_select_gpio(SAFE_MODE);
    gpio_set_direction(SAFE_MODE, GPIO_MODE_INPUT);

    ESP_LOGI(INIT_TAG, "Initializing onboard LED");
    gpio_reset_pin(LED_PIN); 
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  
    gpio_set_level(LED_PIN, 1); 

    //Turn on haptic for 0.5s
    gpio_set_level(HAPTIC_CNTL, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(HAPTIC_CNTL, 0);
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

void initDisplay() {
    i2c_master_init(&dev, DISPLAY_SDA, DISPLAY_SCL, DISPLAY_RESET);
    ssd1306_init(&dev, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ssd1306_bitmaps(&dev, 0,0, electriumLogoBitmap, 128, 64, false);

    // explicitly "push" that buffer to the display
    ssd1306_show_buffer(&dev);

    // Display for 4 seconds with single delay
    ESP_LOGI(INIT_TAG, "Displaying Electrium logo");
    vTaskDelay(pdMS_TO_TICKS(1000));

    ssd1306_clear_screen(&dev, false);
    ssd1306_show_buffer(&dev);
    ESP_LOGI(INIT_TAG, "Display cleared");

    //Signal to display function that initialization is done
    if (displayToScreen != NULL) {
        xTaskNotifyGive(displayToScreen);
    }

    vTaskDelete(NULL);
}