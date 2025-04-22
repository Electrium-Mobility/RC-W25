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

char speedBuffer[26];
char kmBuffer[4] = "km/";
char hBuffer[2] = "h";

char warningBuffer[13] = "Board is off";
char instructionBuffer[15] = "Please turn on";
char boardOffBuffer[10] = "      ";

int remoteBatteryLevel = 0;
int prevRemoteBatteryLevel = -1;
int prevBoardBatteryLevel = -1;
int prevBoardSpeed = -1;
bool prevBoardOn = false;

void display_to_screen()
{
    // Wait as long as necessary for initialization to complete
    ESP_LOGI(DISPLAY_TAG, "Waiting for initialization");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ESP_LOGI(DISPLAY_TAG, "Initialization complete");

    while (1)
    {
        if (safeMode) {
            ssd1306_display_text(&dev, 7, 0, "SAFE", strlen("SAFE"), false);
            ESP_LOGI(DISPLAY_TAG, "Safe mode: %d", safeMode);
        }
        else {
            ssd1306_clear_line(&dev, 7, false);
        }
        //Don't rerender unless the battery level changes
        if (prevRemoteBatteryLevel != remoteBatteryLevel) {
            display_battery(&dev, 0, remoteBatteryLevel, remoteBatteryBuffer, false);
            prevRemoteBatteryLevel = remoteBatteryLevel;
        }
        // Display board battery level, speed, and clear warning messages
        if (boardOn)
        {
            //Don't rerender unless the battery level changes
            if (prevBoardBatteryLevel != boardBatteryLevel) {
                display_battery(&dev, 75, boardBatteryLevel, boardBatteryBuffer, false);
                prevBoardBatteryLevel = boardBatteryLevel;
            }
            //Don't rerender speed unless it changes
            if (prevBoardSpeed != boardSpeed || prevBoardSpeed == -1) {
                ssd1306_clear_line(&dev, 3, false);
                ssd1306_clear_line(&dev, 4, false);
                display_speed(&dev, 25, boardSpeed, false);
                prevBoardSpeed = boardSpeed;
            }
        }
        // Clear speed, board battery level, display warning
        // Don't rerender unless necessary
        else if (!inLightSleep && prevBoardOn)
        {
            ssd1306_display_text(&dev, 0, 75, boardOffBuffer, strlen(boardOffBuffer), false);
            ssd1306_clear_line(&dev, 3, false);
            ssd1306_clear_line(&dev, 4, false);
            ssd1306_clear_line(&dev, 5, false);
            ssd1306_display_text(&dev, 3, 15, warningBuffer, strlen(warningBuffer), false);
            ssd1306_display_text(&dev, 4, 8, instructionBuffer, strlen(instructionBuffer), false);
        }
        prevBoardOn = boardOn;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void fetch_remote_battery()
{
    int raw_reading = adc1_get_raw(BATTERY_PERCENT);
    remoteBatteryLevel = (double)raw_reading / 4095.0 * 100;
}

void display_battery(SSD1306_t *dev, int seg, int battery, char *textBuffer, bool invert)
{
    // Pad with spaces to ensure the value is in a fixed position
    if (battery == 100)
    {
        snprintf(textBuffer, 26 * sizeof(char), "%d%%", battery);
    }
    else if (battery >= 10)
    {
        snprintf(textBuffer, 26 * sizeof(char), "%d%% ", battery);
    }
    else
    {
        snprintf(textBuffer, 26 * sizeof(char), "%d%%  ", battery);
    }
    ssd1306_bitmaps(dev, seg, 0, batteryOutline, 16, 8, false);
    ssd1306_display_text(dev, 0, seg + 20, textBuffer, strlen(textBuffer), false);
    _ssd1306_rectangle(dev, seg + 1, 1, (int)(13 * ((float)battery / 100.0)), 6, false);
}

void display_speed(SSD1306_t *dev, int seg, int speed, bool invert)
{
    // Pad with spaces to ensure fixed position
    if (speed >= 10)
    {
        snprintf(speedBuffer, 26 * sizeof(char), "%d", speed);
    }
    else if (speed < 10)
    {
        snprintf(speedBuffer, 26 * sizeof(char), " %d", speed);
    }
    ssd1306_display_text_x3(dev, 3, seg, speedBuffer, strlen(speedBuffer), false);
    ssd1306_display_text(dev, 3, seg + 50, kmBuffer, strlen(kmBuffer), false);
    ssd1306_display_text(dev, 4, seg + 50, hBuffer, strlen(hBuffer), false);
}