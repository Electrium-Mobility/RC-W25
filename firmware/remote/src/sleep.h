#ifndef LIGHTSLEEP_H
#define LIGHTSLEEP_H

#include "esp_log.h"    
#include "esp_timer.h"  
#include "init.h"

#define SLEEP_TAG "Sleep"

#define INACTIVITY_TIMEOUT 180000000  // 3 min

extern int prevBoardBatteryLevel;

//Called after inactivity timeout
void go_to_sleep(void* arg);

//Check for activity while awake and inactivity timer is running
void check_activity();

//Runs in background, combines both functions above
void light_sleep();

//Timer callback to turn haptic on for one second
void IRAM_ATTR haptic_for_one_second();

//Handles on/off button
void IRAM_ATTR deep_sleep_isr_handler();

#endif