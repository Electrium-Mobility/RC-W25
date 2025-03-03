#ifndef INIT_H
#define INIT_H

#define MAX_CURRENT 54 //Amps
#define WHEEL_DIAMETER 0.15 //15cm
#define MAX_SPEED 50 //km/h

#define INIT_TAG "Initialization"

extern double throttle;
extern int boardSpeed;
extern int boardBatteryLevel;

void init();
void initEspNow();

#endif