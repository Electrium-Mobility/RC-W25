#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "pairing.h"

TaskHandle_t taskHandle;

// void readMacAddress()
// {
//     uint8_t baseMac[6];
//     esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
//     if (ret == ESP_OK)
//     {
//         printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                baseMac[0], baseMac[1], baseMac[2],
//                baseMac[3], baseMac[4], baseMac[5]);
//     }
//     else
//     {
//         ESP_LOGI(TAG, "Failed to read MAC address");
//     }
//     vTaskDelete(NULL);
// }

// void wifi_init()
// {
//     ESP_ERROR_CHECK(nvs_flash_init()); // Required for Wi-Fi
//     ESP_ERROR_CHECK(esp_netif_init()); // Initialize TCP/IP stack
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Set to station mode
//     ESP_ERROR_CHECK(esp_wifi_start());                 // Start Wi-Fi
// }

void app_main()
{
    // while (1) {
    //     int rumble_control_value = get_rumble_control();
    //     int mode_control_value = get_mode_control();
    //     float hall_effect_value = get_hall_effect();
    //     float speed = get_throttle_speed(mode_control_value, hall_effect_value);

    //     vTaskDelay(pdMS_TO_TICKS(1000)); // 1-second loop delay
    // }
    xTaskCreate(pair, "pairing", 4096, NULL, 1, &taskHandle);
}
