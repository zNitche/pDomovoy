#include "../includes/globals.h"

bool g_alarm_triggered = false;
bool g_alarm_standby_init = false;
bool g_alarm_in_standby = false;

bool g_btn_blocked = false;

queue_t g_core0_events_queue;
queue_t g_core1_events_queue;

struct repeating_timer g_status_led_blink_timer;