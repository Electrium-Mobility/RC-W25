#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "speed_control.h"

void app_main() {
    while (1) {
        int rumble_control_value = get_rumble_control();
        int mode_control_value = get_mode_control();
        float hall_effect_value = get_hall_effect();
        float speed = get_throttle_speed(mode_control_value, hall_effect_value);
        
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1-second loop delay
    }
}
