#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "speed_control.h"
#include <math.h>
#include <string.h>

int get_rumble_control(){
    int sensor_value = gpio_get_level(RUMBLE_CNTL);

    if (sensor_value == 1) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    } 
    else {
        ESP_LOGI(SPEED_CONTROL_TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return sensor_value;
}

int get_mode_control(){
    int sensor_value = gpio_get_level(MODE_CNTL);
    if (sensor_value) {
        ESP_LOGI(SPEED_CONTROL_TAG, "Mode Control On. Sensor Value: %d", sensor_value);
    } 
    else {
        ESP_LOGI(SPEED_CONTROL_TAG, "Mode Control Off. Sensor Value: %d", sensor_value);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return sensor_value;
    
}

float get_throttle_speed(int mode_control, float speed_percent){
    float speed_cap = 10.0;
    if (mode_control){
        speed_percent = speed_cap;
    }
    ESP_LOGI(SPEED_CONTROL_TAG, "Speed set to: %d", speed_percent);
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
    double angle = 0;
    char direction[9] = "Neutral";
    double magnitude = 0;

    while (1)
    {
        //Individual readings vary wildly so we take an average
        double raw_analog_A = read_adc_avg(HALL_EFFECT_A, 64);
        double raw_analog_B = read_adc_avg(HALL_EFFECT_B, 64);

        //Sensor measures field strength, so we can use an approxmiation of B = B_0cos(theta)
        //B_0 is field strength at ZERO_POSITION
        //We take inverse cos of the normalized values to figure out angle
        double normalized_A = raw_analog_A / ZERO_POSITION;
        double normalized_B = raw_analog_B / ZERO_POSITION;

        //Make sure the value is within the domain of arccos [-1, 1] to avoid funny business
        double angle_A = (acos(fmax(-1.0, fmin(1.0, normalized_A)))) * 180/M_PI;
        double angle_B = (acos(fmax(-1.0, fmin(1.0, normalized_B)))) * 180/M_PI;

        ESP_LOGI(SPEED_CONTROL_TAG, "Angle A: %.2f, Angle B: %.2f", angle_A, angle_B);

        //2% error around the middle
        if (((fabs(raw_analog_A - ZERO_POSITION) / ZERO_POSITION) < 0.02) ||
            ((fabs(raw_analog_B - ZERO_POSITION) / ZERO_POSITION) < 0.02))
        {
            angle = 0;
            strcpy(direction, "Neutral");
        }
        //Use the value from the sensor it is closer to
        else if (angle_A > angle_B)
        {
            angle = angle_A;
            strcpy(direction, "Forward");
        }
        else if (angle_B > angle_A)
        {
            angle = angle_B;
            strcpy(direction, "Backward");
        }

        magnitude = angle/90.00;

        ESP_LOGI(SPEED_CONTROL_TAG, "Raw Hall A: %.2f Raw Hall B: %.2f Angle: %.2f°\nDirection: %s\n",
                 raw_analog_A, raw_analog_B, angle, direction);
        vTaskDelay(200);
    }
}