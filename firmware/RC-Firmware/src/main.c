#include <stdio.h>
#include "driver/gpio.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define RUMBLE_CNTL GPIO_NUM_12 //Arbitrary Pin I/O

void get_rumble_control(){
    gpio_pad_select_gpio(RUMBLE_CNTL);
    gpio_set_direction(RUMBLE_CNTL, GPIO_MODE_INPUT);

    while (1) {
        int sensor_value = gpio_get_level(RUMBLE_CNTL);

        if (sensor_value == 1) {
            printf("Rumble Control On\n");
        } 
        else {
            printf("Rumble Control Off\n");
        }

        vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    }
}

void app_main() {
    xTaskCreate(get_rumble_control, "get_rumble_control", 2048, NULL, 1, NULL); //Review stack size and task priority
}