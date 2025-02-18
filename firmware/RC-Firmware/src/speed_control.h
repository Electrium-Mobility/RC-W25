#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define RUMBLE_CNTL GPIO_NUM_12 //Arbitrary GPIO 
#define MODE_CNTL GPIO_NUM_13 //Arbitrary GPIO
#define HALL_EFFECT ADC1_CHANNEL_0 //Arbitrary GPIO (ESP32 has ADC on GPI0)
#define HALL_EFFECT_A ADC1_CHANNEL_0 //Arbitrary GPIO for first Hall Effect Sensor
#define HALL_EFFECT_B ADC1_CHANNEL_1 //Arbitrary GPIO for second Hall Effect Sensor

//Raw ADC readings for both sensors in the middle position
//Will tune this during integration they should be the same
#define ZERO_POSITION 200.0

static const char *TAG = "MY_MODULE"; //ESPI_LOG Tag 
int get_rumble_control();
int get_mode_control();
void get_hall_effect(void *pvParameters);
float get_throttle_speed(int mode_control, float speed_percent);

double read_adc_avg(adc1_channel_t channel, int num_samples);
void interpret_hall_readings();

#endif
