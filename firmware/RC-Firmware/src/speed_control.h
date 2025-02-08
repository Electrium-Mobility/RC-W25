#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define RUMBLE_CNTL GPIO_NUM_12 //Arbitrary GPIO 
#define MODE_CNTL GPIO_NUM_13 //Arbitrary GPIO
#define HALL_EFFECT ADC1_CHANNEL_0 //Arbitrary GPIO (ESP32 has ADC on GPI0)

static const char *TAG = "MY_MODULE"; //ESPI_LOG Tag 
int get_rumble_control();
int get_mode_control();
float get_hall_effect();
float get_throttle_speed(int mode_control, float speed_percent);

#endif
