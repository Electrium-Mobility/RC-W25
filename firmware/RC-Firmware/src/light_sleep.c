#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_sleep.h"        
#include "esp_timer.h"  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "light_sleep.h"
#include "init.h"

void go_to_sleep(void* arg) {
    ESP_LOGI(LIGHT_SLEEP_TAG, "Entering light sleep");
    gpio_set_level(LED_PIN, 0); 

    esp_timer_delete(sleep_timer); // Delete the timer to prevent leaks

    //Wake up every 500ms to check if ADC input has changed
    esp_sleep_enable_timer_wakeup(500000);
    while (1) {
        esp_light_sleep_start();

        //Throttle is pushed so we wake up and restart the timer
        if (fabs(adc1_get_raw(HALL_EFFECT) - ZERO_POSITION) > 20){ 
            ESP_LOGI(LIGHT_SLEEP_TAG, "Throttle pushed, waking up");

            //Recreate timer
            esp_timer_create_args_t timer_config = { 
                .callback = &go_to_sleep,    // callback function when it expires
                .name = "SleepTimer"         // Debug name
            };

            esp_timer_create(&timer_config, &sleep_timer); 
            esp_timer_start_once(sleep_timer, INACTIVITY_TIMEOUT); // Starts in one shot mode
            return;
        }
    }
}

void check_activity() {
    if (fabs(adc1_get_raw(HALL_EFFECT) - ZERO_POSITION) > 20){ 
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
