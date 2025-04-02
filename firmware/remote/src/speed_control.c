#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "speed_control.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <math.h>
#include <string.h>
#include "esp_timer.h"

bool prevButtonState = false;
bool safeMode = false;
int debounceDelay = 1000;
int prevTime = 0;
int currentTime = 0;
int debounceTimer = 0;
double throttle = 0;
char direction[9] = "Neutral";

void get_safe_mode(){
    safeMode = gpio_get_level(SAFE_MODE);

    if (safeMode != prevButtonState) {
        debounceTimer = 0;
    }
    else {
        currentTime = esp_timer_get_time();
        debounceTimer += currentTime - prevTime;
    }
    prevTime = currentTime;
    if (safeMode && (debounceTimer > debounceDelay)) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Safe Mode On. Sensor Value: %d", safeMode);
    } 
    else if (debounceTimer > debounceDelay) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Safe Mode Off. Sensor Value: %d", safeMode);
    }

    prevButtonState = safeMode;
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay    

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
    double angle = 0; //Angle between magnet and zero position

    while (1) {
        //Individual readings vary wildly so we take an average
        double raw_analog = read_adc_avg(THROTTLE_CNTL, 10);
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