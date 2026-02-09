#include "../includes/handlers.h"

#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "pico/multicore.h"

void toggle_alarm_standby() {
    if (!alarm_triggered) {
        if (!alarm_in_standby) {
            multicore_launch_core1(core_1);
        } else {
            multicore_reset_core1();
        }

        alarm_in_standby = !alarm_in_standby;
    }
}
