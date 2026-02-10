#include "../includes/delegates.h"

#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "pico/multicore.h"
#include "../includes/debug_print.h"

void enable_alarm_standby() {
    if (!g_alarm_in_standby) {
        multicore_launch_core1(core_1);
        g_alarm_in_standby = true;

        debug_print("enabled alarm standby\n");
    }
}

void disable_triggered_alarm() {
    multicore_reset_core1();

    g_alarm_in_standby = false;
    g_alarm_triggered = false;

    debug_print("disabled triggered alarm\n");
}
