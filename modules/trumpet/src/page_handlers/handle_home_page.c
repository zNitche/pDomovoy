#include "../../includes/page_handlers/handle_home_page.h"

#include "../../includes/bitmaps/battery_icon.h"
#include "../../includes/bitmaps/bell_icon_big.h"
#include "../../includes/bitmaps/celcius_degree_icon.h"
#include "../../includes/bitmaps/humidity_icon.h"
#include "../../includes/bitmaps/thermometer_icon.h"
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

SSD1306_Bitmap thermometer_icon = {.width = 8, .height = 8, .data = NULL};
SSD1306_Bitmap humidity_icon = {.width = 8, .height = 8, .data = NULL};
SSD1306_Bitmap celcius_degree_icon = {.width = 8, .height = 8, .data = NULL};
SSD1306_Bitmap battery_icon = {.width = 8, .height = 8, .data = NULL};
SSD1306_Bitmap bell_icon = {.width = 32, .height = 32, .data = NULL};

void __load_bitmaps() {
    if (thermometer_icon.data == NULL) {
        ssd1306_load_bitmap(thermometer_icon_bitmap, &thermometer_icon);
    }

    if (celcius_degree_icon.data == NULL) {
        ssd1306_load_bitmap(celcius_degree_icon_bitmap, &celcius_degree_icon);
    }

    if (humidity_icon.data == NULL) {
        ssd1306_load_bitmap(humidity_icon_bitmap, &humidity_icon);
    }

    if (battery_icon.data == NULL) {
        ssd1306_load_bitmap(battery_icon_bitmap, &battery_icon);
    }

    if (bell_icon.data == NULL) {
        ssd1306_load_bitmap(bell_icon_big_bitmap, &bell_icon);
    }
}

void handle_home_page(SSD1306_Frame* frame) {
    static float readings[2] = {0.0};

    __load_bitmaps();

    ssd1306_insert_bitmap(frame, 0, 12, &thermometer_icon);
    ssd1306_insert_bitmap(frame, 0, 22, &humidity_icon);

    aht20_get_measurements(g_aht20_i2c, readings);

    char temp_str[4];
    char humidity_str[5];

    snprintf(temp_str, sizeof(temp_str), "%d", (int)readings[0]);
    snprintf(humidity_str, sizeof(humidity_str), "%d%%", (int)readings[1]);

    ssd1306_render_string(frame, 10, 12, temp_str, 2, false);
    ssd1306_insert_bitmap(frame, 31, 12, &celcius_degree_icon);

    ssd1306_insert_bitmap(frame, 0, 22, &humidity_icon);
    ssd1306_render_string(frame, 10, 22, humidity_str, 2, false);

    ssd1306_insert_bitmap(frame, 0, 34, &battery_icon);
    ssd1306_render_string(frame, 10, 34, "@T 3.3V", 1, false);

    ssd1306_insert_bitmap(frame, 0, 46, &battery_icon);
    ssd1306_render_string(frame, 10, 46, "@W 3.3V", 1, false);

    ssd1306_insert_bitmap(frame, 88, 20, &bell_icon);
}
