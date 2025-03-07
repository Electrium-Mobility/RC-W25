#include "init.h"
#include "vesc_comm.h"
#include "VescUart.h"
#include "math.h"
#include <string.h>

int boardSpeed;
int boardBatteryLevel;
int maxRpm;


//Here is where you read the battery level of the longboard
//Extract inpVoltage from vescData (It's defined in init.h and updated constantly)
//0% battery is around 3V, max battery is around 5V
//DON'T scale linearly, figure out how to map these values to a percentage
//There is a diagram in the doc but it's not perfect for our specifications


void read_battery_percent() {

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