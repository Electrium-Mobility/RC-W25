#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_sleep.h"        
#include "esp_timer.h"  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "Main";

// For this example code, assume the throttle is a digital value
#define THROTTLE_PIN GPIO_NUM_4 
#define LED_PIN GPIO_NUM_2 // built-in
#define INACTIVITY_TIMEOUT 180000000  // 3 min

esp_timer_handle_t sleep_timer;  // reference to the timer 


void go_to_sleep(void* arg) {
    ESP_LOGI(TAG, "Entering deep sleep");
    gpio_set_level(LED_PIN, 0); 

    esp_timer_delete(sleep_timer); // Delete the timer to prevent leaks

    // Configure wakeup source 
    esp_sleep_enable_ext0_wakeup(THROTTLE_PIN, 1);
    esp_deep_sleep_start();
}

void check_activity() {
     
    if(gpio_get_level(THROTTLE_PIN) == 1){ 
        ESP_LOGI(TAG, "THROTTLE PUSHED");
        
        // Restart timer only if it's still running
        esp_timer_stop(sleep_timer);
        esp_timer_start_once(sleep_timer, INACTIVITY_TIMEOUT);
    }
}

void app_main() {

    ESP_LOGI(TAG, "System started");
    
    // Re/Initialize LED
    gpio_reset_pin(LED_PIN); 
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);  
    gpio_set_level(LED_PIN, 1);  

    // Re/Initialize Button
    gpio_set_direction(THROTTLE_PIN, GPIO_MODE_INPUT);  
    gpio_set_pull_mode(THROTTLE_PIN, GPIO_PULLDOWN_ONLY);

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
