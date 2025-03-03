#ifndef VESC_COMM
#define VESC_COMM

#define VESC_COMM_TAG "VESC-COMM"

int maxRpm;

void read_battery_percent();
void apply_throttle();
void determine_speed();

#endif