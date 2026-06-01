#include "../../includes/page_handlers/handle_home_page.h"

#include "../../includes/bitmaps/battery_icon.h"
#include "../../includes/bitmaps/bell_icon_big.h"
#include "../../includes/bitmaps/celcius_degree_icon.h"
#include "../../includes/bitmaps/humidity_icon.h"
#include "../../includes/bitmaps/thermometer_icon.h"
#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pdomovoy_common/helpers.h"
#include "pdomovoy_common/math.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>
#include <string.h>

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

void __handle_env_sensors(SSD1306_Frame* frame, float aht20_readings[2]) {
    ssd1306_insert_bitmap(frame, 0, 12, &thermometer_icon);
    ssd1306_insert_bitmap(frame, 0, 22, &humidity_icon);

    char temp_str[4];
    char humidity_str[5];

    snprintf(temp_str, sizeof(temp_str), "%d", (int)aht20_readings[0]);
    snprintf(humidity_str, sizeof(humidity_str), "%d%%",
             (int)aht20_readings[1]);

    ssd1306_render_string(frame, 10, 12, temp_str, 2, false);
    ssd1306_insert_bitmap(frame, 31, 12, &celcius_degree_icon);

    ssd1306_insert_bitmap(frame, 0, 24, &humidity_icon);
    ssd1306_render_string(frame, 10, 24, humidity_str, 2, false);
}

void __render_battery_details_row(SSD1306_Frame* frame, uint8_t y, char* label,
                                  float voltage) {
    char battery_voltage_str[5];

    ssd1306_insert_bitmap(frame, 0, y, &battery_icon);
    ssd1306_render_string(frame, 10, y, label, 1, false);

    snprintf(battery_voltage_str, sizeof(battery_voltage_str), "%.1fV",
             voltage);
    ssd1306_render_string(frame, 30, y, battery_voltage_str, 1, false);
}

void __handle_batteries(SSD1306_Frame* frame) {
    char trumpet_battery_voltage_str[8];
    char warden_battery_voltage_str[8];

    __render_battery_details_row(frame, 36, "@T", g_battery_voltage);

    if (g_warden_remote_data.connected) {
        __render_battery_details_row(frame, 48, "@W",
                                     g_warden_remote_data.battery_voltage);
    }
}

void __handle_alarm_bell(SSD1306_Frame* frame) {
    if (g_alarm_state != ALARM_STATE_STANDBY &&
        g_alarm_state != ALARM_STATE_STANDBY_INIT &&
        g_alarm_state != ALARM_STATE_TRIGGERED) {
        return;
    }

    char alarm_state_str[6];

    ssd1306_insert_bitmap(frame, 88, 15, &bell_icon);

    if (g_alarm_state == ALARM_STATE_STANDBY_INIT) {
        strcpy(alarm_state_str, "SBY_I");
    }

    if (g_alarm_state == ALARM_STATE_STANDBY) {
        strcpy(alarm_state_str, "STDBY");
    }

    if (g_alarm_state == ALARM_STATE_TRIGGERED) {
        strcpy(alarm_state_str, "TRIGG");
    }

    ssd1306_render_string(frame, 80, 50, alarm_state_str, 1, false);
}

void __get_aht20_readings(float output[2]) {
    static const uint32_t interval_ms = 1000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    aht20_get_measurements(g_aht20_i2c, output);
}

void handle_home_page(SSD1306_Frame* frame) {
    static float aht20_readings[2] = {0.0};

    __get_aht20_readings(aht20_readings);

    __load_bitmaps();

    __handle_env_sensors(frame, aht20_readings);
    __handle_batteries(frame);
    __handle_alarm_bell(frame);
}
