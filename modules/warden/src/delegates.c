#include "../includes/delegates.h"

#include "../includes/core_1.h"
#include "../includes/debug_print.h"
#include "../includes/globals.h"
#include "pico/multicore.h"

void init_alarm_standby() {
    if (g_alarm_state == ALARM_STATE_NONE) {
        g_alarm_state = ALARM_STATE_STANDBY_INIT;

        debug_print("alarm standby init \n");
    }
}

void enable_alarm_standby() {
    if (g_alarm_state != ALARM_STATE_STANDBY) {
        multicore_launch_core1(core_1);
        g_alarm_state = ALARM_STATE_STANDBY;

        debug_print("enabled alarm standby\n");
    }
}

void disable_triggered_alarm() {
    g_alarm_state = ALARM_STATE_NONE;

    // reset on disarm
    g_detected_low_battery_voltage = false;

    multicore_reset_core1();

    debug_print("disabled triggered alarm\n");
}
