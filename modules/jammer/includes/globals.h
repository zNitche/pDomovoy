#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>

// should be accessed only from core_0

extern bool g_alarm_triggered;
extern bool g_alarm_in_standby;
extern bool g_alarm_standby_init;

extern bool g_btn_blocked;

extern float g_battery_voltage;

extern queue_t g_core0_events_queue;
extern queue_t g_core1_events_queue;

extern repeating_timer_t g_onboard_led_blink_timer;
extern repeating_timer_t g_status_led_blink_timer;

extern repeating_timer_t g_adc_voltmeter_timer;
