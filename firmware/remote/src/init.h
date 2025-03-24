#ifndef INIT_H
#define INIT_H

#define HALL_EFFECT ADC1_CHANNEL_2
#define BATTERY_PERCENT ADC1_CHANNEL_3
#define SAFE_MODE GPIO_NUM_4
#define HAPTIC_CNTL GPIO_NUM_1
#define LED_PIN GPIO_NUM_2 // built-in

#define DISPLAY_SDA GPIO_NUM_6
#define DISPLAY_SCL GPIO_NUM_7
#define DISPLAY_RESET -1  // Use -1 if no reset pin is connected

#define ZERO_POSITION 200.0 //Raw ADC value for zero position

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define INIT_TAG "Initialization"

extern double throttle;
extern bool safeMode;
extern char direction[9];
extern int boardSpeed;
extern int remoteBatteryLevel;
extern int boardBatteryLevel;

extern TaskHandle_t displayToScreen;

void init();
void initADC();
void initGPIO();
void initEspNow();
void initDisplay();

#endif