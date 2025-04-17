#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "semaphore.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "VescUart.h"

#include "init.h"

SemaphoreHandle_t pairingMutex;
dataPackage vescData;

void init() {
    initEspNow();
    initVescUart();
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

void initVescUart() {
    //Map TWAIRX and TWAITX to UART1
    gpio_reset_pin(GPIO_NUM_0);  //IO0
    gpio_reset_pin(GPIO_NUM_1);  //IO1

    //Map IO0, IO1 to TX, RX of UART1
    uart_set_pin(UART_NUM, GPIO_NUM_0, GPIO_NUM_1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    //Initialize UART
    uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = 122,
	};

	ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
	ESP_ERROR_CHECK(uart_set_pin(UART_NUM, 0, 1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM, 256, 256, 0, NULL, 0));
}