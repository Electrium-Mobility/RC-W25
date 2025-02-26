#ifndef DISPLAY_H
#define DISPLAY_H

extern SSD1306_t dev;

void display_to_screen();
void fetch_remote_battery();

//Displays battery icon and text
void display_battery(SSD1306_t *dev, int seg, int battery, char *textBuffer, bool invert);

#endif