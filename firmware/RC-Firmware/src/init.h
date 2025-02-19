#ifndef INIT_H
#define INIT_H

#define RUMBLE_CNTL GPIO_NUM_12 //Arbitrary GPIO 
#define MODE_CNTL GPIO_NUM_11
#define HALL_EFFECT_A ADC1_CHANNEL_0
#define HALL_EFFECT_B ADC1_CHANNEL_1
#define THROTTLE_PIN GPIO_NUM_4 
#define LED_PIN GPIO_NUM_2 // built-in

static const char* INIT_TAG = "Initialization";

void init();

#endif