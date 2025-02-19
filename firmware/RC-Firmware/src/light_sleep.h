#ifndef LIGHTSLEEP_H
#define LIGHTSLEEP_H

#include "esp_log.h"    
#include "esp_timer.h"  
#include "init.h"

#define LIGHT_SLEEP_TAG "Light Sleep"

#define INACTIVITY_TIMEOUT 180000000  // 3 min

esp_timer_handle_t sleep_timer;  // reference to the timer 

//Called after inactivity timeout
void go_to_sleep(void* arg);

//Check for activity while awake and inactivity timer is running
void check_activity();

//Runs in background, combines both functions above
void light_sleep();

#endif