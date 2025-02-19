#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include "driver/adc.h"
#include "init.h"

//Raw ADC readings for both sensors in the middle position
//Will tune this during integration they should be the same
#define SPEED_CONTROL_TAG "Speed Control"

int get_rumble_control();
int get_mode_control();
float get_throttle_speed(int mode_control, float speed_percent);

double read_adc_avg(adc1_channel_t channel, int num_samples);
void interpret_hall_readings();

#endif
