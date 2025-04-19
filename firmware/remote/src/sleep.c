#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_sleep.h"        
#include "esp_timer.h"  
#include "esp_wifi.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sleep.h"
#include "init.h"
#include <math.h>

esp_timer_handle_t light_sleep_timer;  // reference to the timer 

void go_to_sleep(void* arg) {
    ESP_LOGI(SLEEP_TAG, "Entering light sleep");
    gpio_set_level(LED_PIN, 0); 

    //2 haptic pulses of 0.5s
    gpio_set_level(HAPTIC_CNTL, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(HAPTIC_CNTL, 0);
    gpio_set_level(HAPTIC_CNTL, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    gpio_set_level(HAPTIC_CNTL, 0);

    esp_timer_delete(light_sleep_timer); // Delete the timer to prevent leaks

    //Wake up every 500ms to check if ADC input has changed
    esp_sleep_enable_timer_wakeup(500000);
    while (1) {
        esp_light_sleep_start();

        //Throttle is pushed so we wake up and restart the timer
        if (fabs(adc1_get_raw(THROTTLE_CNTL) - ZERO_POSITION) > 100){ 
            ESP_LOGI(SLEEP_TAG, "Throttle pushed, waking up");

            //Recreate timer
            esp_timer_create_args_t timer_config = { 
                .callback = &go_to_sleep,    // callback function when it expires
                .name = "SleepTimer"         // Debug name
            };

            esp_timer_create(&timer_config, &light_sleep_timer); 
            esp_timer_start_once(light_sleep_timer, INACTIVITY_TIMEOUT); // Starts in one shot mode
            
        //Reset wifi connection
        esp_wifi_stop();
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
        vTaskDelay(pdMS_TO_TICKS(100));
        return;
        }
    }
}

void check_activity() {
    if (fabs(adc1_get_raw(THROTTLE_CNTL) - ZERO_POSITION) > 100){ 
        ESP_LOGI(SLEEP_TAG, "THROTTLE PUSHED");
        
        // Restart timer only if it's still running
        esp_timer_stop(light_sleep_timer);
        esp_timer_start_once(light_sleep_timer, INACTIVITY_TIMEOUT);
    }
}

void light_sleep() {
    ESP_LOGI(SLEEP_TAG, "System started");
    // Configuiring Struct of Timer
    esp_timer_create_args_t timer_config = { 
        .callback = &go_to_sleep,    // callback function when it expires
        .name = "SleepTimer"         // Debug name
    };

    esp_timer_create(&timer_config, &light_sleep_timer); 
    esp_timer_start_once(light_sleep_timer, INACTIVITY_TIMEOUT); // Starts in one shot mode

    while(1) {
        check_activity();  // Check button every 100ms
        vTaskDelay(100 / portTICK_PERIOD_MS); 
    }
}

void setup_timer() {
    timer_config_t config = {
        .divider = 80,
        .counter_dir = TIMER_COUNT_UP,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = false,
        .intr_type = TIMER_INTR_LEVEL
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 1000000); //Run for 1 second
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, (void*)haptic_for_one_second, NULL, 0);
}

void IRAM_ATTR haptic_for_one_second() {
    gpio_set_level(HAPTIC_CNTL, 0);
    timer_pause(TIMER_GROUP_0, TIMER_0);
}

void IRAM_ATTR deep_sleep_isr_handler() {
    ESP_LOGI(SLEEP_TAG, "Going into deep sleep");
    //Haptic pulse for 1s
    gpio_set_level(HAPTIC_CNTL, 1);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_start(TIMER_GROUP_0, TIMER_0);

    esp_sleep_enable_ext1_wakeup((1ULL << ON_OFF), ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_start();
}
