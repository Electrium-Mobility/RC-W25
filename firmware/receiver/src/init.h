#ifndef INIT_H
#define INIT_H

#include "VescUart.h"

#define MAX_CURRENT 54 //Amps
#define WHEEL_DIAMETER 0.15 //15cm
#define MAX_SPEED 50 //km/h

#define INIT_TAG "Initialization"

#define UART_NUM UART_NUM_1

extern double throttle;
extern int boardSpeed;
extern int boardBatteryLevel;
extern dataPackage* data;

void init();
void initEspNow();
void initVescUart();

#endif