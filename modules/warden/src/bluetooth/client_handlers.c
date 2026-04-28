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

    debug_print("found service with uuid16: %d\n",
                ble_service_context.service.uuid16);

    if (ble_service_context.service.uuid16 != PD_GATT_PRIMARY_SERVICE_UUID16) {
        return;
    }

    debug_print("storing service: uuid16 %d, start: %d, end: %d\n",
                ble_service_context.service.uuid16,
                ble_service_context.service.start_group_handle,
                ble_service_context.service.end_group_handle);
}

void __handle_gatt_event_characteristic_query_result(uint8_t* packet) {
    gatt_event_characteristic_query_result_get_characteristic(
        packet, &ble_service_context.characteristic);

    debug_print("storing characteristic: uuid16 %d\n",
                ble_service_context.characteristic.uuid16);

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_CONFIGURED);
}

void __handle_gatt_event_query_complete(uint8_t* packet) {
    switch (pd_gatt_client_state) {
    case PD_GATT_CLIENT_STATE_UNSET:
        gatt_client_discover_characteristics_for_service_by_uuid16(
            handle_gatt_client_event, ble_service_context.connection_handle,
            &ble_service_context.service, PD_WARDEN_VERSION_GATT_VALUE_HANDLE);

        break;

    case PD_GATT_CLIENT_STATE_CONFIGURED:
        const uint8_t res =
            gatt_client_write_value_of_characteristic_without_response(
                ble_service_context.connection_handle,
                ble_service_context.characteristic.value_handle,
                sizeof(WARDEN_VERSION), WARDEN_VERSION);

        debug_print("sent warden version with res: %d\n", res);

        break;

    default:
        break;
    }

    debug_print("query complete\n");
}
