#include "init.h"
#include "vesc_comm.h"
#include "VescUart.h"
#include "math.h"
#include <string.h>

int boardSpeed;
int boardBatteryLevel;
int maxRpm;


/* Voltage-SOC graph mappings 
 * 
 * These are just eyeballed mappings; where the i'th index of 
 * soc_lookup_table corresponds to i'th index of voltage_lookup_table.
 * fetch_battery_percent() uses linear interpolation to estimate exact
 * value within points.
 * 
*/ 
#define NUM_POINTS 20

float soc_lookup_table[NUM_POINTS] = {100.0, 97.5, 95.0, 92.5, 90.0, 87.5, 85.0, 82.5, 80.0, 77.5, 
    75.0, 72.5, 70.0, 67.5, 65.0, 62.5, 60.0, 57.5, 55.0, 52.5, 
    50.0, 47.5, 45.0, 42.5, 40.0, 37.5, 35.0, 32.5, 30.0, 27.5, 
    25.0, 22.5, 20.0, 17.5, 15.0, 12.5, 10.0, 7.5, 5.0, 2.5, 0.0};

float voltage_lookup_table[NUM_POINTS] = {41.5, 40.2, 39.5, 39.3, 39.1, 39.0, 38.8, 38.65, 38.5, 38.25, 
    38.0, 37.7, 37.5, 37.4, 37.1, 36.9, 36.6, 36.5, 36.1, 35.9, 35.75
    35.6, 35.5, 35.25, 34.9, 34.75, 34.6, 34.5, 34.3, 34.2, 34.0,
    33.85, 33.6, 33.5, 33.4, 33.0, 32.5, 32.2, 32.0, 31.0, 30.3, 29.5};



//Here is where you read the battery level of the longboard
//Extract inpVoltage from vescData (It's defined in init.h and updated constantly)
//0% battery is around 3V, max battery is around 5V
//DON'T scale linearly, figure out how to map these values to a percentage
//There is a diagram in the doc but it's not perfect for our specifications
void read_battery_percent(){
    //read voltage from dataPackage struct
    float raw_voltage = dataPackage->inpVoltage;

    //map raw value to percentage
    if (raw_voltage >= voltage_lookup_table[0]) return 100.0;
    if (raw_voltage <= voltage_lookup_table[NUM_POINTS - 1]) return 0.0;

    for (int i = 0; i < NUM_POINTS - 1; i++) {
        if (raw_voltage >= lookup_table[i + 1]) {
            // apply linear interpolation formula
            float v1 = voltage_lookup_table[i], v2 = voltage_lookup_table[i + 1];
            float soc1 = soc_lookup_table[i], soc2 = soc_lookup_table[i + 1];
            boardBatteryLevel = soc1 + ((raw_voltage - v1) * (soc2 - soc1) / (v2 - v1));
        }
    }
}

//Here is where you send data to the ESC
//Rearrange the formula from the doc to solve for rpm, and set that as a max rpm
//There is a variable called maxRpm defined in vesc_comm.h
//Take the max rpm, multiply it by the throttle, and send it over
//MAX_SPEED and WHEEL_DIAMETER are defined in init.h but you can use them here
void apply_throttle() {
    maxRpm = (MAX_SPEED * 1000) / (M_PI * WHEEL_DIAMETER * 60);
    double scaledRpm = throttle * maxRpm;
    if (strcmp(direction, "Backward") == 0) {
        scaledRpm *= -1;
    }
    setRPM(vescData, scaledRpm);
}

//Here is where you modify boardSpeed using the formula from the doc
//Get the rpm from vescData (It's defined in init.h and updated constantly)
//MAX_SPEED and WHEEL_DIAMETER are defined in init.h but you can use them here
void determine_speed() {
    double board_rpm = vescData->rpm;
    double speed = board_rpm*M_PI*WHEEL_DIAMETER*60/1000
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    boardSpeed = speed;
}

void fetchVesc() {
    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        getVescValues(vescData);
    }
}