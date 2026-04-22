#include "../../includes/page_handlers/page_env_sensors.h"

#include <stdio.h>
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"

void handle_env_sensors_page(SSD1306_Frame* frame) {
    static float readings[2] = {0.0};

    aht20_get_measurements(g_aht20_i2c, readings);

    char temp_str[9];
    char humidity_str[9];

    snprintf(temp_str, sizeof(temp_str), "temp= %d", (int)readings[0]);
    ssd1306_render_string(frame, 0, 13, temp_str, 2, true);

    snprintf(temp_str, sizeof(temp_str), "humi= %d", (int)readings[1]);
    ssd1306_render_string(frame, 0, 22, temp_str, 2, true);
}
