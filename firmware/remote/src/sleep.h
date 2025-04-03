#ifndef LIGHTSLEEP_H
#define LIGHTSLEEP_H

#include "esp_log.h"    
#include "esp_timer.h"  
#include "init.h"

#define SLEEP_TAG "Light Sleep"

#define INACTIVITY_TIMEOUT 180000000  // 3 min

//Called after inactivity timeout
void go_to_sleep(void* arg);

//Check for activity while awake and inactivity timer is running
void check_activity();

//Runs in background, combines both functions above
void light_sleep();

//Handles on/off button
void deep_sleep_check();

#endif