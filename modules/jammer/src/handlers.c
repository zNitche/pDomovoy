#include "../includes/handlers.h"

#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "pico/multicore.h"

void toggle_alarm_standby() {
    if (!g_alarm_in_standby) {
        multicore_launch_core1(core_1);
    } else {
        multicore_reset_core1();
    }

    g_alarm_in_standby = !g_alarm_in_standby;
}

void disable_triggered_alarm() {
    multicore_reset_core1();

    g_alarm_in_standby = false;
    g_alarm_triggered = false;
}
