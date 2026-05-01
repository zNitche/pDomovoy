#include "../includes/delegates.h"

#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "../includes/bluetooth/core.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/types.h"
#include "pico/multicore.h"

void set_alarm_state(enum AlarmState state) {
    debug_print("[AlarmState] alarm_state set to %d\n", state);

    g_alarm_state = state;

    pd_clear_queue(&g_bt_functions_queue);
}

void init_alarm_standby() {
    if (g_alarm_state == ALARM_STATE_NONE) {
        set_alarm_state(ALARM_STATE_STANDBY_INIT);
        debug_print("[AlarmState] alarm standby init \n");
    }
}

void enable_alarm_standby() {
    if (g_alarm_state != ALARM_STATE_STANDBY) {
        multicore_launch_core1(core_1);

        set_alarm_state(ALARM_STATE_STANDBY);
        debug_print("[AlarmState] enabled alarm standby\n");
    }
}

void disable_triggered_alarm() {
    set_alarm_state(ALARM_STATE_NONE);

    // reset on disarm
    g_detected_low_battery_voltage = false;

    multicore_reset_core1();

    debug_print("[AlarmState] disabled triggered alarm\n");
}
