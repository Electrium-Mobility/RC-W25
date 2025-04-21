#ifndef PAIRING_H
#define PAIRING_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_now.h"
#include "semaphore.h"
#include "init.h"

#define PAIRING_TAG "ESP-NOW-COMM"

extern SemaphoreHandle_t pairingMutex;
extern int boardSpeed;
extern int boardBatteryLevel;

//Data to be sent over ESP-NOW
typedef struct remote_data {
    double throttle;      // 0% - 100% throttle from potentiometer
    short direction; // Direction
} remote_data;

typedef struct board_data {
    int boardBatteryLevel;     // battery levels from longboard
    int boardSpeed;         // Speed of longboard
} board_data;

void readMacAddress();

// callback for receiving data
void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len);

// callback for sending data
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);

void pair();
#endif