#include "init.h"
#include "vesc_comm.h"
#include "VescUart.h"
#include "math.h"
#include <string.h>
#include "esp_log.h"

int boardSpeed;
int boardBatteryLevel;
int maxRpm;
double board_rpm;
double prevRpm;

/* Voltage-SOC graph mappings 
 * 
 * These are just eyeballed mappings; where the i'th index of 
 * soc_lookup_table corresponds to i'th index of voltage_lookup_table.
 * fetch_battery_percent() uses linear interpolation to estimate exact
 * value within points.
 * 
*/ 

float soc_lookup_table[NUM_POINTS] = {100.0, 97.5, 95.0, 92.5, 90.0, 87.5, 85.0, 82.5, 80.0, 77.5, 
    75.0, 72.5, 70.0, 67.5, 65.0, 62.5, 60.0, 57.5, 55.0, 52.5, 
    50.0, 47.5, 45.0, 42.5, 40.0, 37.5, 35.0, 32.5, 30.0, 27.5, 
    25.0, 22.5, 20.0, 17.5, 15.0, 12.5, 10.0, 7.5, 5.0, 2.5, 0.0};

float voltage_lookup_table[NUM_POINTS] = {41.5, 40.2, 39.5, 39.3, 39.1, 39.0, 38.8, 38.65, 38.5, 38.25, 
    38.0, 37.7, 37.5, 37.4, 37.1, 36.9, 36.6, 36.5, 36.1, 35.9, 35.75,
    35.6, 35.5, 35.25, 34.9, 34.75, 34.6, 34.5, 34.3, 34.2, 34.0,
    33.85, 33.6, 33.5, 33.4, 33.0, 32.5, 32.2, 32.0, 31.0, 30.3};

void fetch_vesc() {
    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        getVescValues(&vescData);
    }
}

void read_battery_percent()
{
    while (1) {
    // read voltage from dataPackage struct
    float raw_voltage = vescData.inpVoltage;
    ESP_LOGI(VESC_COMM_TAG, "%.2f", raw_voltage);

    // map raw value to percentage
    if (raw_voltage >= voltage_lookup_table[0])
    {
        boardBatteryLevel = 100.0;
    }
    if (raw_voltage <= voltage_lookup_table[NUM_POINTS - 1])
    {
        boardBatteryLevel = 0.0;
    }

    for (int i = 0; i < NUM_POINTS - 1; i++)
    {
        if (raw_voltage >= voltage_lookup_table[i + 1])
        {
            // apply linear interpolation formula
            float v2 = voltage_lookup_table[i], v1 = voltage_lookup_table[i + 1];
            float soc2 = soc_lookup_table[i], soc1 = soc_lookup_table[i + 1];
            boardBatteryLevel = soc1 + ((raw_voltage - v1) * (soc2 - soc1) / (v2 - v1));
            break;
        }
    }
    ESP_LOGI(VESC_COMM_TAG, "%d", boardBatteryLevel);
    vTaskDelay(pdMS_TO_TICKS(10000));
}
}

void apply_throttle()
{
    maxRpm = (MAX_SPEED * 1000) / (M_PI * WHEEL_DIAMETER * 60);
    while (1)
    {
        double scaledRpm = throttle * maxRpm * 7.0;
        if (direction == -1)
        {
            scaledRpm *= -0.4;
        }
        //Brake gently
        if (fabs(scaledRpm) < MIN_ERPM && fabs(prevRpm) > 0) {
            setBrakeCurrent(&vescData, 1.0);
        }
        //Apply desired rpm, we can't set it to 0 since it will jerk
        else if (fabs(scaledRpm) >= MIN_ERPM) {
            setRPM(&vescData, scaledRpm);
        }
    }
}

void determine_speed()
{
    while (1) {
        board_rpm = vescData.rpm / 7.0;
        double speed = board_rpm * M_PI * WHEEL_DIAMETER * 60 / 1000;
        ESP_LOGI(VESC_COMM_TAG, "Board RPM: %.2f, Board Speed: %.2f", board_rpm, speed);
    
        boardSpeed = (int)fabs(speed);
    
        ESP_LOGI(VESC_COMM_TAG, "boardSpeed: %d", boardSpeed);
        prevRpm = board_rpm;
        vTaskDelay(pdMS_TO_TICKS(100));
}
}