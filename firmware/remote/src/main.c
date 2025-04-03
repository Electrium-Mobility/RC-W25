#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "init.h"
#include "pairing.h"
#include "speed_control.h"
#include "sleep.h"
#include "display.h"

TaskHandle_t initialization;
TaskHandle_t pairing;
TaskHandle_t interpretHallReadings;
TaskHandle_t lightSleep;
TaskHandle_t displayToScreen;

void app_main()
{
    xTaskCreate(init, "initialization", 2048, NULL, 1, &initialization);
    // xTaskCreate(readMacAddress, "Mac", 4096, NULL, 1, &macAddress);
    xTaskCreate(pair, "pairing", 4096, NULL, 1, &pairing);
    xTaskCreate(interpret_hall_readings, "interpret", 2048, NULL, 1, &interpretHallReadings);
    xTaskCreate(light_sleep, "light_sleep", 2048, NULL, 1, &lightSleep);
    xTaskCreate(display_to_screen, "Display", 4096, NULL, 1, &displayToScreen);
}
