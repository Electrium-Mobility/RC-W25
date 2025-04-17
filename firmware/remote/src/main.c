#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "init.h"
#include "pairing.h"
#include "speed_control.h"
#include "sleep.h"
#include "display.h"
#include "charging.h"

TaskHandle_t initialization;
TaskHandle_t pairing;
TaskHandle_t interpretThrottleReadings;
TaskHandle_t lightSleep;
TaskHandle_t displayToScreen;
TaskHandle_t chargingLed;

void app_main()
{
    xTaskCreate(init, "initialization", 2048, NULL, 1, &initialization);
    // xTaskCreate(readMacAddress, "Mac", 4096, NULL, 1, &macAddress);
    xTaskCreate(pair, "pairing", 4096, NULL, 1, &pairing);
    xTaskCreate(read_throttle_value, "interpret", 2048, NULL, 1, &interpretThrottleReadings);
    xTaskCreate(light_sleep, "light_sleep", 2048, NULL, 1, &lightSleep);
    xTaskCreate(display_to_screen, "Display", 4096, NULL, 1, &displayToScreen);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(ON_OFF, deep_sleep, NULL);
}
