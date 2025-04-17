#ifndef VESC_COMM
#define VESC_COMM

#define VESC_COMM_TAG "VESC-COMM"

#define NUM_POINTS 41

extern dataPackage vescData;
extern char direction[9];
extern double throttle;

void read_battery_percent();
void apply_throttle();
void determine_speed();

//Update VESC values like rpm, input voltage, etc. every 100ms
void fetch_vesc();

#endif