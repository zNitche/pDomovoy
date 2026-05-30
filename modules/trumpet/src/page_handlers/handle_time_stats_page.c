#include "../../includes/page_handlers/handle_time_stats_page.h"

#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pdomovoy_common/helpers.h"
#include "pdomovoy_common/version.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

void __timestamp_to_hhmm(uint32_t timestamp, SimpleTimeData* time_data) {
    uint32_t total_seconds = timestamp / 1000;
    uint32_t total_minutes = total_seconds / 60;

    time_data->hours = total_minutes / 60;
    time_data->minutes = total_minutes % 60;
}

void __get_boot_time_data(SimpleTimeData* time_data) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());

    __timestamp_to_hhmm(current_time_ms, time_data);
}

void __get_warden_connection_time_data(SimpleTimeData* time_data) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t timestamp =
        to_ms_since_boot(get_absolute_time()) - g_warden_connected_timestamp;

    __timestamp_to_hhmm(timestamp, time_data);
}

void __get_alarm_armed_time_data(SimpleTimeData* time_data) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t timestamp =
        to_ms_since_boot(get_absolute_time()) - g_alarm_armed_timestamp;

    __timestamp_to_hhmm(timestamp, time_data);
}

void __render_row(SSD1306_Frame* frame, uint8_t y, char* label,
                  SimpleTimeData* time_data) {
    char time_data_char[6];
    char target_string;

    snprintf(time_data_char, sizeof(time_data_char), "%02u:%02u",
             time_data->hours, time_data->minutes);

    ssd1306_render_string(frame, 0, y, label, 1, false);
    ssd1306_render_string(frame, 82, y, time_data_char, 1, false);
}

void handle_time_stats_page(SSD1306_Frame* frame) {
    static SimpleTimeData time_data_since_boot = {.hours = 0, .minutes = 0};
    static SimpleTimeData time_data_since_warden_connected = {.hours = 0,
                                                              .minutes = 0};
    static SimpleTimeData time_data_since_alarm_armed = {.hours = 0,
                                                         .minutes = 0};

    __get_boot_time_data(&time_data_since_boot);
    __get_warden_connection_time_data(&time_data_since_warden_connected);
    __get_alarm_armed_time_data(&time_data_since_alarm_armed);

    __render_row(frame, 16, "T booted", &time_data_since_boot);

    if (g_warden_connected) {
        __render_row(frame, 28, "W conn", &time_data_since_warden_connected);

        if (g_alarm_armed_timestamp != 0) {
            __render_row(frame, 40, "Armed @", &time_data_since_alarm_armed);
        }
    }
}
