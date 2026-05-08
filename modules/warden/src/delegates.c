#include "../includes/delegates.h"

#include "../includes/bluetooth/core.h"
#include "../includes/core_1.h"
#include "../includes/globals.h"
#include "pd_common_config.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/types.h"
#include "pico/multicore.h"

void set_alarm_state(enum AlarmState state) {
    debug_print("[AlarmState] alarm_state set to %d\n", state);

    g_alarm_state = state;

    if (PD_WARDEN_BT_MODE_ENABLED) {
        pd_bt_process_queue_prio_func(pd_bt_send_alarm_state);
    }
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

        g_alarm_armed = true;
        set_alarm_state(ALARM_STATE_STANDBY);

        debug_print("[AlarmState] armed alarm\n");
    }
}

void disarm_alarm() {
    multicore_reset_core1();

    queue_free(&g_core0_events_queue);
    queue_free(&g_core1_events_queue);

    set_alarm_state(ALARM_STATE_NONE);

    g_detected_low_battery_voltage = false;
    g_alarm_armed = false;

    debug_print("[AlarmState] disarmed alarm\n");
}
