#include "ssd1306.h"
#include <driver/i2c.h>
#include "display.h"

void display_to_screen() {
    SSD1306_t dev;
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&dev, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "Custom RC", 14, false);
}