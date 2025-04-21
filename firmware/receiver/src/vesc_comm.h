#ifndef VESC_COMM
#define VESC_COMM

#define VESC_COMM_TAG "VESC-COMM"

#define NUM_POINTS 41
#define MIN_ERPM 900

extern dataPackage vescData;
extern short direction;
extern double throttle;

void read_battery_percent();
void apply_throttle();
void determine_speed();

//Update VESC values like rpm, input voltage, etc. every 100ms
void fetch_vesc();

#endif