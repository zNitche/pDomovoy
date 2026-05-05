#include "../../includes/bluetooth/client_handlers.h"

#include "../../includes/bluetooth/core.h"
#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/helpers.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/defines.h"
#include "pdomovoy_common/version.h"
#include "pico/stdlib.h"

void __handle_gatt_event_service_query_result(uint8_t* packet) {
    gatt_event_service_query_result_get_service(packet,
                                                &ble_service_context.service);

    debug_print("[GATT_CLIENT] found service with uuid16: 0x%04x\n",
                ble_service_context.service.uuid16);

    if (ble_service_context.service.uuid16 != PD_GATT_PRIMARY_SERVICE_UUID16) {
        return;
    }

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_CHARS_DISCOVERY);

    debug_print(
        "[GATT_CLIENT] storing service: uuid16 0x%04x, start: %d, end: %d\n",
        ble_service_context.service.uuid16,
        ble_service_context.service.start_group_handle,
        ble_service_context.service.end_group_handle);
}

void __handle_gatt_event_characteristic_query_result(uint8_t* packet) {
    switch (pd_gatt_client_state) {
    case PD_GATT_CLIENT_STATE_CHARS_DISCOVERY:
        gatt_client_characteristic_t tmp_chararacteristic;

        gatt_event_characteristic_query_result_get_characteristic(
            packet, &tmp_chararacteristic);

        const uint16_t char_uuid16 = tmp_chararacteristic.uuid16;

        if (char_uuid16 != pd_characteristic_discovery_context.char_uuid16) {
            debug_print("[GATT_CLIENT] characteristic uuid16 mismatch, "
                        "expected: 0x%04x, got: 0x%04x\n",
                        char_uuid16,
                        pd_characteristic_discovery_context.char_uuid16);

            return;
        }

        memcpy(pd_characteristic_discovery_context.target_char,
               &tmp_chararacteristic, sizeof(gatt_client_characteristic_t));

        debug_print("[GATT_CLIENT] storing characteristic: uuid16 0x%04x\n",
                    char_uuid16);

        break;
    default:
        break;
    }
}

void __handle_gatt_event_query_complete(uint8_t* packet) {
    switch (pd_gatt_client_state) {
    case PD_GATT_CLIENT_STATE_PROCESSING:
        update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);

        break;

    default:
        debug_print(
            "[GATT_CLIENT] gatt_event_query_complete client state: %d\n",
            pd_gatt_client_state);
        break;
    }
}

void __handle_gatt_notification_event(uint8_t* packet) {
    uint16_t value_handle = gatt_event_notification_get_value_handle(packet);

    debug_print("[GATT_CLIENT] GATT_EVENT_NOTIFICATION 0x%04X\n", value_handle);

    switch (value_handle) {
    case PD_TRUMPET_ALARM_STATE_GATT_NOTIFICATION_HANDLE:
        __handle_alarm_state_gatt_notification(packet);
        break;
    default:
        break;
    }
}

void __handle_alarm_state_gatt_notification(uint8_t* packet) {
    uint16_t value_length = gatt_event_notification_get_value_length(packet);
    const uint8_t* value = gatt_event_notification_get_value(packet);

    int alarm_state;
    memcpy(&alarm_state, value, sizeof(int));

    if (g_alarm_state == ALARM_STATE_STANDBY &&
        alarm_state == ALARM_STATE_NONE) {
        g_alarm_disarm_requested = true;
    } else {
        g_alarm_state = alarm_state;
    }

    debug_print("[GATT_CLIENT] GATT_EVENT_NOTIFICATION : alarm state %d\n",
                alarm_state);
}
