#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

#include "init.h"
#include "pairing.h"
#include "vesc_comm.h"
#include "VescUart.h"

TaskHandle_t fetchVescValues;

void app_main() {
    xTaskCreate(fetchVesc, "Update VESC values", 4096, NULL, 1, &fetchVescValues);
}