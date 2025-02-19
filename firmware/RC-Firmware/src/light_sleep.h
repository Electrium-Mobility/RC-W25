#ifndef LIGHTSLEEP_H
#define LIGHTSLEEP_H

#include "esp_log.h"    
#include "esp_timer.h"  
#include "init.h"

#define LIGHT_SLEEP_TAG "Light Sleep"

#define INACTIVITY_TIMEOUT 180000000  // 3 min

esp_timer_handle_t sleep_timer;  // reference to the timer 

void go_to_sleep(void* arg);
void check_activity();
void light_sleep();

#endif