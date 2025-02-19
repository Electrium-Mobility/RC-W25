#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_sleep.h"        
#include "esp_timer.h"  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "light_sleep.h"

void go_to_sleep(void* arg) {
    ESP_LOGI(LIGHT_SLEEP_TAG, "Entering light sleep");
    gpio_set_level(LED_PIN, 0); 

    esp_timer_delete(sleep_timer); // Delete the timer to prevent leaks

    // Configure wakeup source 
    esp_sleep_enable_ext0_wakeup(THROTTLE_PIN, 1);
    esp_light_sleep_start();
}

void check_activity() {
    if (gpio_get_level(THROTTLE_PIN) == 1){ 
        ESP_LOGI(LIGHT_SLEEP_TAG, "THROTTLE PUSHED");
        
        // Restart timer only if it's still running
        esp_timer_stop(sleep_timer);
        esp_timer_start_once(sleep_timer, INACTIVITY_TIMEOUT);
    }
}

void light_sleep() {
    ESP_LOGI(LIGHT_SLEEP_TAG, "System started");
    // Configuiring Struct of Timer
    esp_timer_create_args_t timer_config = { 
        .callback = &go_to_sleep,    // callback function when it expires
        .name = "SleepTimer"         // Debug name
    };

    esp_timer_create(&timer_config, &sleep_timer); 
    esp_timer_start_once(sleep_timer, INACTIVITY_TIMEOUT); // Starts in one shot mode

    while(1) {
        check_activity();  // Check button every 100ms
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}
