#include "charging.h"
#include "init.h"
#include "driver/gpio.h"

void setLED() {
    //Turn off an LED if one was already on
    if (prevLED > 0) {
        gpio_set_level(prevLED, 0);
    }

    if (gpio_get_level(CHARGE_EN) == 1) {

        int stat1 = gpio_get_level(CHARGE_STAT1);
        int stat2 = gpio_get_level(CHARGE_STAT2);

        //Red LED
        if (stat1 == 0) {
            gpio_set_level(CHARGE_RED_LED, 1);
            prevLED = CHARGE_RED_LED;

        }
        //Green LED
        else if (stat1 == 1 && stat2 == 1) {
            gpio_set_level(CHARGE_GREEN_LED, 1);
            prevLED = CHARGE_GREEN_LED;
        }
        //Orange LED
        else {
            gpio_set_level(CHARGE_ORANGE_LED, 1);
            prevLED = CHARGE_ORANGE_LED;
        }

    }
    //Not charging so disable all LEDs
    else {
        prevLED = 0;
    }
}