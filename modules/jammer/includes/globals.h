#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>

extern bool g_alarm_triggered;
extern bool g_alarm_in_standby;

extern queue_t g_core0_events_queue;
extern queue_t g_core1_events_queue;

extern struct repeating_timer g_status_led_blink_timer;
