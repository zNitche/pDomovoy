#include "../../includes/bluetooth/helpers.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"
#include <string.h>

void __pd_setup_gap_advertisements() {
    uint8_t adv_data[] = {0x02,
                          BLUETOOTH_DATA_TYPE_FLAGS,
                          0x06,
                          0x17,
                          BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
                          'T',
                          'R',
                          'U',
                          'M',
                          'P',
                          'E',
                          'T'};

    uint16_t adv_int_min = 800;
    uint16_t adv_int_max = 800;
    uint8_t adv_type = 0;

    bd_addr_t null_addr;
    memset(null_addr, 0, 6);

    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t*)adv_data);
    gap_advertisements_enable(1);

    debug_print("[GATT_SERVER] ble advertisement active\n");
}

void __pd_client_state_cleanup() {
    g_warden_connected = false;
    g_warden_connected_timestamp = 0;
    g_alarm_armed_timestamp = 0;

    g_warden_battery_voltage = 0;
    memset(g_warden_version, 0, sizeof(g_warden_version));

    g_alarm_state = ALARM_STATE_NONE;

    debug_print("[GATT_SERVER] PD client state cleanup\n");
}

void toggle_alarm_state(enum AlarmState state) {
    g_alarm_state = state;

    debug_print("[GATT_SERVER] setting alarm state to: %d\n", state);

    if (ble_notification_enabled) {
        att_server_request_can_send_now_event(ble_connection_handle);

        debug_print(
            "[GATT_SERVER] triggered ble notification for alarm state: %d\n",
            state);
    }
}
