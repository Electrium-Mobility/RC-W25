#ifndef SPEED_CONTROL_H
#define SPEED_CONTROL_H

#include "driver/adc.h"
#include "init.h"

#define SPEED_CONTROL_TAG "Speed Control"
#define ZERO_POSITION_VOLTAGE 1.65

void get_safe_mode();

double read_adc_avg(adc1_channel_t channel, int num_samples);
void read_throttle_value();

#endif
