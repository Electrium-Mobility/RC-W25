#ifndef INIT_H
#define INIT_H

#define HALL_EFFECT ADC1_CHANNEL_2
#define BATTERY_PERCENT ADC1_CHANNEL_3
#define SAFE_MODE ADC1_CHANNEL_4
#define HAPTIC_CNTL ADC1_CHANNEL_6
#define LED_PIN GPIO_NUM_2 // built-in

//These display pins are not 100% confirmed yet they're just placeholders
#define DISPLAY_SDA 8
#define DISPLAY_SCL 9
#define DISPLAY_RESET -1  // Use -1 if no reset pin is connected

#define ZERO_POSITION 200.0 //Raw ADC value for zero position

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define INIT_TAG "Initialization"

extern double throttle;
extern bool safeMode;
extern int boardRpm;
extern int remoteBatteryLevel;
extern int boardBatteryLevel;

extern TaskHandle_t displayToScreen;

void init();
void initADC();
void initGPIO();
void initEspNow();
void initDisplay();

#endif