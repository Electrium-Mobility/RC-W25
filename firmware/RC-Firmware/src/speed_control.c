#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "speed_control.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <math.h>
#include <string.h>

bool safeMode = false;
double throttle = 0;

int get_rumble_control(){
    int sensor_value = gpio_get_level(HAPTIC_CNTL);

    if (sensor_value == 1) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    } 
    else {
        ESP_LOGI(SPEED_CONTROL_TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return sensor_value;
}

void get_safe_mode(){
    safeMode = gpio_get_level(SAFE_MODE);

    if (safeMode) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Safe Mode On. Sensor Value: %d", safeMode);
    } 
    else {
        ESP_LOGI(SPEED_CONTROL_TAG, "Safe Mode Off. Sensor Value: %d", safeMode);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay    
}

float get_throttle_speed(int mode_control, float speed_percent){
    float speed_cap = 10.0;
    if (mode_control){
        speed_percent = speed_cap;
    }
    ESP_LOGI(SPEED_CONTROL_TAG, "Speed set to: %.2f", speed_percent);
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return speed_percent;
}

double read_adc_avg(adc1_channel_t channel, int num_samples) {
    double sum = 0;
    for (int i = 0; i < num_samples; i++) {
        sum += (double)adc1_get_raw(channel);
    }
    return sum / (double)num_samples;
}

// We will tune exact values to fit possible ranges once we start integration
void interpret_hall_readings()
{
    adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT, ADC_ATTEN_DB_12); //3.3V range

    double angle = 0; //Angle between magnet and zero position
    char direction[9] = "Neutral";

    while (1) {
        //Individual readings vary wildly so we take an average
        double raw_analog = read_adc_avg(HALL_EFFECT, 10);
        double raw_voltage = (raw_analog/4095.0) * 3.3;

        //5% tolerance around the zero point in either direction
        if (fabs((raw_voltage - ZERO_POSITION_VOLTAGE)/ZERO_POSITION_VOLTAGE) < 0.05) {
            strcpy(direction, "Neutral");
        }
        else if (raw_voltage > ZERO_POSITION_VOLTAGE) {
            strcpy(direction, "Forward");
        }
        else if (raw_voltage < ZERO_POSITION_VOLTAGE) {
            strcpy(direction, "Backward");
        }

        //Most magnetic flux through sensor at 90 degrees from zero position
        //B = B_0 * cos(theta) where B_0 is the max magnetic flux, theta is angle away from B_0 point
        //This will give us 90 degrees for zero position, 0 degrees at max throttle
        angle = 90 - acos((fabs(raw_voltage - ZERO_POSITION_VOLTAGE))/ZERO_POSITION_VOLTAGE) * 180/M_PI;
        
        throttle = angle/90.0;

        vTaskDelay(200);
    }
}