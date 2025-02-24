#ifndef INIT_H
#define INIT_H

#define HALL_EFFECT ADC1_CHANNEL_2
#define BATTERY_PERCENT ADC1_CHANNEL_3
#define SAFE_MODE ADC1_CHANNEL_4
#define HAPTIC_CNTL ADC1_CHANNEL_6
#define LED_PIN GPIO_NUM_2 // built-in

static const char* INIT_TAG = "Initialization";
extern double ZERO_POSITION = 200.0;

void init();

#endif