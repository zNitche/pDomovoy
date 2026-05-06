#include "../../includes/page_handlers/handle_time_stats_page.h"

#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pdomovoy_common/helpers.h"
#include "pdomovoy_common/version.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

void __timestamp_to_hhmm(uint32_t timestamp, uint32_t* hours,
                         uint32_t* minutes) {
    uint32_t total_seconds = timestamp / 1000;
    uint32_t total_minutes = total_seconds / 60;

    *hours = total_minutes / 60;
    *minutes = total_minutes % 60;
}

void __get_boot_time_data(uint32_t* hours, uint32_t* minutes) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());

    __timestamp_to_hhmm(current_time_ms, hours, minutes);
}

void __get_warden_connection_time_data(uint32_t* hours, uint32_t* minutes) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t timestamp =
        to_ms_since_boot(get_absolute_time()) - g_warden_connected_timestamp;

    __timestamp_to_hhmm(timestamp, hours, minutes);
}

void __get_g_alarm_armed_time_data(uint32_t* hours, uint32_t* minutes) {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    const uint32_t timestamp =
        to_ms_since_boot(get_absolute_time()) - g_alarm_armed_timestamp;

    __timestamp_to_hhmm(timestamp, hours, minutes);
}

void handle_time_stats_page(SSD1306_Frame* frame) {
    static uint32_t hours_since_boot = 0;
    static uint32_t minutes_since_boot = 0;
    static uint32_t hours_since_warden_connected = 0;
    static uint32_t minutes_since_warden_connected = 0;
    static uint32_t hours_since_alarm_armed = 0;
    static uint32_t minutes_since_alarm_armed = 0;

    static char time_data_since_boot_str[15];
    static char time_data_since_warden_connected_str[13];
    static char time_data_since_alarm_armed_str[13];

    __get_boot_time_data(&hours_since_boot, &minutes_since_boot);
    __get_warden_connection_time_data(&hours_since_warden_connected,
                                      &minutes_since_warden_connected);
    __get_g_alarm_armed_time_data(&hours_since_alarm_armed,
                                  &minutes_since_alarm_armed);

    snprintf(time_data_since_boot_str, sizeof(time_data_since_boot_str),
             "T booted %u:%u", hours_since_boot, minutes_since_boot);

    ssd1306_render_string(frame, 0, 16, time_data_since_boot_str, 1, false);

    if (g_warden_connected) {
        snprintf(time_data_since_warden_connected_str,
                 sizeof(time_data_since_warden_connected_str), "W conn %u:%u",
                 hours_since_warden_connected, minutes_since_warden_connected);

        ssd1306_render_string(frame, 0, 28,
                              time_data_since_warden_connected_str, 1, false);

        if (g_alarm_armed_timestamp != 0) {
            snprintf(time_data_since_alarm_armed_str,
                     sizeof(time_data_since_alarm_armed_str), "Armed %u:%u",
                     hours_since_alarm_armed, minutes_since_alarm_armed);

            ssd1306_render_string(frame, 0, 40, time_data_since_alarm_armed_str,
                                  1, false);
        }
    }
}
