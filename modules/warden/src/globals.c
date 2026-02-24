#include "../includes/globals.h"
#include "../includes/types.h"

// should be accessed only from core_0

enum AlarmState g_alarm_state = ALARM_STATE_NONE;

bool g_btn_blocked = false;

float g_battery_voltage = 0.0;

bool g_detected_low_battery_voltage = false;
bool g_sensor_error = false;

queue_t g_core0_events_queue;
queue_t g_core1_events_queue;

repeating_timer_t g_onboard_led_blink_timer;
repeating_timer_t g_status_led_blink_timer;

repeating_timer_t g_adc_voltmeter_timer;