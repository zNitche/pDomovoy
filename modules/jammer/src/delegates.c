#include "../includes/delegates.h"

#include "../includes/core_1.h"
#include "../includes/debug_print.h"
#include "../includes/globals.h"
#include "pico/multicore.h"

void init_alarm_standby() {
    if (!g_alarm_in_standby && !g_alarm_triggered && !g_alarm_standby_init) {
        g_alarm_standby_init = true;

        debug_print("alarm standby init \n");
    }
}

void enable_alarm_standby() {
    if (!g_alarm_in_standby) {
        multicore_launch_core1(core_1);
        g_alarm_in_standby = true;

        debug_print("enabled alarm standby\n");
    }
}

void disable_triggered_alarm() {
    g_alarm_standby_init = false;
    g_alarm_in_standby = false;
    g_alarm_triggered = false;

    multicore_reset_core1();

    debug_print("disabled triggered alarm\n");
}
