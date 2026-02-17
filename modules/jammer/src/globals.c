#include "../includes/globals.h"

// should be accessed only from core_0

bool g_alarm_triggered = false;
bool g_alarm_standby_init = false;
bool g_alarm_in_standby = false;

bool g_btn_blocked = false;

float g_battery_voltage = 0.0;

queue_t g_core0_events_queue;
queue_t g_core1_events_queue;

repeating_timer_t g_onboard_led_blink_timer;
repeating_timer_t g_status_led_blink_timer;

repeating_timer_t g_adc_voltmeter_timer;