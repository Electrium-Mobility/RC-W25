#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

#include "init.h"
#include "pairing.h"
#include "vesc_comm.h"
#include "VescUart.h"

TaskHandle_t initializeHandle;
TaskHandle_t espNowHandle;
TaskHandle_t fetchVescValuesHandle;
TaskHandle_t fetchBatteryPercentHandle;
TaskHandle_t applyThrottleHandle;
TaskHandle_t determineSpeedHandle;

TaskHandle_t testHandle;

void app_main() {
    // xTaskCreate(testPins, "Test Pins", 2048, NULL, 1, &testHandle);
    init();
    xTaskCreate(pair, "ESP-NOW functionality", 4096, NULL, 1, &espNowHandle);
    xTaskCreate(fetch_vesc, "Update VESC values", 4096, NULL, 1, &fetchVescValuesHandle);
    xTaskCreate(read_battery_percent, "Read board battery percent", 4096, NULL, 1, &fetchBatteryPercentHandle);
    xTaskCreate(apply_throttle, "Apply throttle", 4096, NULL, 1, &applyThrottleHandle);
    xTaskCreate(determine_speed, "Determine speed of board", 4096, NULL, 1, &determineSpeedHandle);
}