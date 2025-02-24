#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "init.h"
#include "pairing.h"
#include "speed_control.h"
#include "light_sleep.h"

TaskHandle_t initialization;
TaskHandle_t pairing;
TaskHandle_t interpretHallReadings;
TaskHandle_t lightSleep;

void app_main()
{
    xTaskCreate(init, "initialization", 2048, NULL, 1, &initialization);
    xTaskCreate(pair, "pairing", 4096, NULL, 1, &pairing);
    xTaskCreate(interpret_hall_readings, "interpret", 2048, NULL, 1, &interpretHallReadings);
    xTaskCreate(light_sleep, "light_sleep", 2048, NULL, 1, &lightSleep);
}
