#ifndef PAIRING_H
#define PAIRING_H

#include <stdio.h>
#include <string.h>
#include "esp_now.h"
#include "semaphore.h"
#include "init.h"

#define PAIRING_TAG "ESP-NOW-COMM"

extern SemaphoreHandle_t pairingMutex;

//Data to be sent both ways over ESP-NOW
typedef struct struct_data {
    double throttle;      // 0% - 100% throttle from potentiometer
    int boardBatteryLevel;     // battery levels from longboard
    int boardSpeed;         // Speed of longboard
    short direction; // Direction
} struct_data;

void readMacAddress();

// callback for receiving data
void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len);

// callback for sending data
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);

void pair();
#endif