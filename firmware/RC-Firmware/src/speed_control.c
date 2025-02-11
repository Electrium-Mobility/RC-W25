#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "speed_control.h"

int get_rumble_control(){
    gpio_pad_select_gpio(RUMBLE_CNTL);
    gpio_set_direction(RUMBLE_CNTL, GPIO_MODE_INPUT);

    int sensor_value = gpio_get_level(RUMBLE_CNTL);

    if (sensor_value == 1) {
        ESP_LOGI(TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    } 
    else {
        ESP_LOGI(TAG, "Rumble Control On. Sensor Value: %d", sensor_value);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return sensor_value;
}

float get_hall_effect(){
    adc1_config_width(ADC_WIDTH_BIT_12); //12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT, ADC_ATTEN_DB_11);
    
    int raw_analog_value = adc1_get_raw(HALL_EFFECT);
    
    float speed_percent = (raw_analog_value / 4095.0) * 100.0;  // Convert to percentage
        
    ESP_LOGI(TAG, "ADC Raw: %d, Speed Percent: %.2f%%", raw_analog_value, speed_percent);
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return speed_percent;
}

int get_mode_control(){
    gpio_pad_select_gpio(MODE_CNTL);
    gpio_set_direction(MODE_CNTL, GPIO_MODE_INPUT);
    int sensor_value = gpio_get_level(MODE_CNTL);
    if (sensor_value) {
        ESP_LOGI(TAG, "Mode Control On. Sensor Value: %d", sensor_value);
    } 
    else {
        ESP_LOGI(TAG, "Mode Control Off. Sensor Value: %d", sensor_value);
    }
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return sensor_value;
    
}

float get_throttle_speed(int mode_control, float speed_percent){
    float speed_cap = 10.0;
    if (mode_control){
        speed_percent = speed_cap;
    }
    ESP_LOGI(TAG, "Speed set to: %d", speed_percent);
    vTaskDelay(pdMS_TO_TICKS(10)); //Arbitary 0.01 sec delay
    return speed_percent;
}

float get_hall_magnitude(){
    adc1_config_width(ADC_WIDTH_BIT_12); //12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT_A, ADC_ATTEN_DB_11);

    adc1_config_width(ADC_WIDTH_BIT_12); //12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT_B, ADC_ATTEN_DB_11);
    
    int raw_analog_A = adc1_get_raw(HALL_EFFECT_A);
    int raw_analog_B = adc1_get_raw(HALL_EFFECT_B);

    float speed_percent_A = (raw_analog_A / 4095.0) * 100.0;  // Convert to percentage
    float speed_percent_B = (raw_analog_B / 4095.0) * 100.0;  // Convert to percentage

    float magnitude = (speed_percent_A + speed_percent_B) / 2.0;

    ESP_LOGI(TAG, "Raw Hall A: %d Raw Hall B: %d Magnitude: %.2f%%", raw_analog_A, raw_analog_B, magnitude);

    return magnitude;
}

int get_hall_direction(){
    adc1_config_width(ADC_WIDTH_BIT_12); //12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT_A, ADC_ATTEN_DB_11);

    adc1_config_width(ADC_WIDTH_BIT_12); //12-bit (0-4095) B/c 2^12 = 4096
    adc1_config_channel_atten(HALL_EFFECT_B, ADC_ATTEN_DB_11);
    
    int raw_analog_A = adc1_get_raw(HALL_EFFECT_A);
    int raw_analog_B = adc1_get_raw(HALL_EFFECT_B);

    if(raw_analog_A > raw_analog_B){
        ESP_LOG(TAG, "Direction: Forward");
        return 1; //move forward
    }
    else if(raw_analog_A < raw_analog_B){
        ESP_LOG(TAG, "Direction: Backward");
        return -1; //move backward
    }
    else{
        ESP_LOGI(TAG, "Direction: Neutral");
        return 0; 
    }
}