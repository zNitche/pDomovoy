#include "../../includes/bluetooth/client_handlers.h"

#include "../../includes/bluetooth/core.h"
#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/helpers.h"
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

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);

    debug_print(
        "[GATT_CLIENT] storing service: uuid16 0x%04x, start: %d, end: %d\n",
        ble_service_context.service.uuid16,
        ble_service_context.service.start_group_handle,
        ble_service_context.service.end_group_handle);
}

void __handle_gatt_event_characteristic_query_result(uint8_t* packet) {
    switch (pd_gatt_client_state) {
    case PD_GATT_CLIENT_STATE_GET_CHAR:
        gatt_event_characteristic_query_result_get_characteristic(
            packet, &pd_gatt_action_context.target_char);

        const uint16_t char_uuid16 = pd_gatt_action_context.target_char.uuid16;

        debug_print("[GATT_CLIENT] storing characteristic: uuid16 0x%04x\n",
                    char_uuid16);

        update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY_TO_PROCESS_CHAR);

        // proceed action to char
        const uint8_t res =
            gatt_client_write_value_of_characteristic_without_response(
                ble_service_context.connection_handle,
                pd_gatt_action_context.target_char.value_handle,
                pd_gatt_action_context.value_length,
                pd_gatt_action_context.value);

        debug_print(
            "[GATT_CLIENT] sent '%d' bytes to char '0x%04x' with res: %u\n",
            pd_gatt_action_context.value_length, char_uuid16, res);

        // handle error
        if (res != 0) {
            update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);
        }

        break;
    default:
        break;
    }
}

void __handle_gatt_event_query_complete(uint8_t* packet) {
    switch (pd_gatt_client_state) {
    case PD_GATT_CLIENT_STATE_READY_TO_PROCESS_CHAR:
        update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);

        break;

    default:
        break;
    }
}
