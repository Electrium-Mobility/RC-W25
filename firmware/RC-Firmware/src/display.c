#include <stdio.h>
#include "ssd1306.h"
#include <driver/i2c.h>
#include <driver/adc.h>
#include "esp_log.h"
#include "display.h"
#include "init.h"
#include <string.h>
#include "batteryOutline.h"

char remoteBatteryBuffer[26];
char boardBatteryBuffer[26];

void display_to_screen() {
	//Wait as long as necessary for initialization to complete
	ESP_LOGI(DISPLAY_TAG, "Waiting for initialization");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ESP_LOGI(DISPLAY_TAG, "Initialization complete");

    snprintf(remoteBatteryBuffer, 26*sizeof(char), "%d%%", remoteBatteryLevel);
    ssd1306_clear_screen(&dev, false);
    display_battery(&dev, 0, remoteBatteryLevel, remoteBatteryBuffer, false);
    display_battery(&dev, 80, boardBatteryLevel, boardBatteryBuffer, false);
}

void fetch_remote_battery() {
    int raw_reading = adc1_get_raw(BATTERY_PERCENT);
    remoteBatteryLevel = (double)raw_reading/4095.0 * 100;
}

void display_battery(SSD1306_t *dev, int seg, int battery, char *textBuffer, bool invert) {
    snprintf(textBuffer, 26 * sizeof(char), "%d%%", battery);
    ssd1306_bitmaps(dev, seg, 0, batteryStencil, 16, 8, false);
    ssd1306_display_text(dev, 0, seg + 20, textBuffer, strlen(textBuffer), false);
    _ssd1306_rectangle(dev, seg + 1, 1, (int)(13 * ((float)battery / 100.0)), 6, false);
}