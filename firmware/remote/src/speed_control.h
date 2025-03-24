#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include "driver/adc.h"
#include "init.h"

//Raw ADC readings for both sensors in the middle position
//Will tune this during integration they should be the same
#define SPEED_CONTROL_TAG "Speed Control"
#define ZERO_POSITION_VOLTAGE 1.65 //Taken from datasheet (VCC/2)

void get_safe_mode();

double read_adc_avg(adc1_channel_t channel, int num_samples);
void interpret_hall_readings();

#endif
