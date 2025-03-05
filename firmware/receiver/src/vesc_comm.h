#ifndef VESC_COMM
#define VESC_COMM

#define VESC_COMM_TAG "VESC-COMM"

extern dataPackage *vescData;

void read_battery_percent();
void apply_throttle();
void determine_speed();

//Update VESC values like rpm, input voltage, etc. every 100ms
void fetchVesc();

#endif