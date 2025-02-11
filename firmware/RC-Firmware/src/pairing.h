#ifndef PAIRING_H
#define PAIRING_H

#include <stdio.h>
#include <string.h>
#include "esp_now.h"

#define TAG "ESP-NOW-COMM"

// arbitrary struct for testing
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

// struct to transfer data
/*
typedef struct struct_data {
    float input_signal;      // 0% - 100% throttle from hall sensor
    float battery_level;     // battery levels from longboard
    int rpm_reading;         // vesc rpm rating
} struct_data;
*/

// callback for receiving data
void on_data_recv(const esp_now_recv_info_t *recv_info, const uint8_t *incoming_data, int len);

// callback for sending data
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status);

void pair();
#endif