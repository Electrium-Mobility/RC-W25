#include <stdio.h>
#include "ssd1306.h"
#include <driver/i2c.h>
#include <driver/adc.h>
#include "display.h"
#include "init.h"
#include <string.h>

int remoteBatteryLevel = 0;
char remoteBatteryBuffer[26];

void display_to_screen() {
    snprintf(remoteBatteryBuffer, 26*sizeof(char), "%d%%", remoteBatteryLevel);
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, remoteBatteryBuffer, strlen(remoteBatteryBuffer), false);
}

void fetch_remote_battery() {
    int raw_reading = adc1_get_raw(BATTERY_PERCENT);
    remoteBatteryLevel = (double)raw_reading/4095.0 * 100;
}