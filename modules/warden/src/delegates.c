#include "../includes/delegates.h"

#include "../includes/bluetooth/core.h"
#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/types.h"
#include "pico/multicore.h"

void set_alarm_state(enum AlarmState state) {
    debug_print("[AlarmState] alarm_state set to %d\n", state);

    g_alarm_state = state;

    pd_bt_process_queue_prio_func(pd_bt_send_alarm_state);
}

void init_alarm_standby() {
    if (g_alarm_state == ALARM_STATE_NONE) {
        set_alarm_state(ALARM_STATE_STANDBY_INIT);
        debug_print("[AlarmState] alarm standby init \n");
    }
}

void arm_alarm() {
    if (g_alarm_state != ALARM_STATE_STANDBY) {
        multicore_launch_core1(core_1);

        g_alarm_disarm_requested = false;
        set_alarm_state(ALARM_STATE_STANDBY);

        debug_print("[AlarmState] enabled alarm standby\n");
    }
}

void disarm_alarm() {
    // reset on disarm
    g_detected_low_battery_voltage = false;
    multicore_reset_core1();

    set_alarm_state(ALARM_STATE_NONE);

    debug_print("[AlarmState] disabled triggered alarm\n");
}
