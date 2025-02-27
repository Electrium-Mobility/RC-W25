#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"

#define DISPLAY_TAG "SSD1306"

extern SSD1306_t dev;
extern TaskHandle_t displayToScreen;
extern int boardBatteryLevel;
extern int remoteBatteryLevel;

//Handle what to display during normal operation
void display_to_screen();

void fetch_remote_battery();

//Displays battery icon and text
void display_battery(SSD1306_t *dev, int seg, int battery, char *textBuffer, bool invert);

#endif