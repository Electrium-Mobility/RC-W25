#ifndef INIT_H
#define INIT_H

#include "VescUart.h"

#define MAX_CURRENT 54 //Amps
#define WHEEL_DIAMETER 0.15 //15cm
#define MAX_SPEED 50 //km/h

#define INIT_TAG "Initialization"

#define UART_NUM UART_NUM_0

#define TX_PIN GPIO_NUM_2
#define RX_PIN GPIO_NUM_3

void init();
void testPins();
void initLogs();
void initEspNow();
void initVescUart();

#endif