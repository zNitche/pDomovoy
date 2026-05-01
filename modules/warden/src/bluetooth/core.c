#include "../../includes/bluetooth/core.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/handlers.h"
#include "../../includes/bluetooth/helpers.h"
#include "../../includes/bluetooth/types.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pd_common_config.h"
#include "pdomovoy_common/bluetooth.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/defines.h"
#include "pdomovoy_common/helpers.h"
#include "pdomovoy_common/version.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

int pd_bt_send_version_code() {
    pd_gatt_action_context.target_char = &pd_gatt_warden_version_characteristic;
    pd_gatt_action_context.value = WARDEN_VERSION;
    pd_gatt_action_context.value_length = sizeof(WARDEN_VERSION);

    pd_start_gatt_action();

    return 0;
}

int pd_bt_send_battery_voltage() {
    static uint8_t voltage_buff[sizeof(float)];
    memcpy(voltage_buff, &g_battery_voltage, sizeof(float));

    pd_gatt_action_context.target_char =
        &pd_gatt_warden_battery_voltage_characteristic;
    pd_gatt_action_context.value = voltage_buff;
    pd_gatt_action_context.value_length = sizeof(float);

    pd_start_gatt_action();

    return 0;
}

void pd_gatt_get_characteristic(uint16_t uuid16,
                                gatt_client_characteristic_t* characteristic) {
    debug_print("[pd_gatt_get_characteristic] getting 0x%04x\n", uuid16);

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_CHARS_DISCOVERY);

    pd_characteristic_discovery_context.char_uuid16 = uuid16;
    pd_characteristic_discovery_context.target_char = characteristic;

    gatt_client_discover_characteristics_for_service_by_uuid16(
        __pd_handle_gatt_client_event, ble_service_context.connection_handle,
        &ble_service_context.service, uuid16);
}

void pd_start_gatt_action() {
    // update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY_TO_PROCESS_CHAR);

    const uint8_t res =
        gatt_client_write_value_of_characteristic_without_response(
            ble_service_context.connection_handle,
            pd_gatt_action_context.target_char->value_handle,
            pd_gatt_action_context.value_length, pd_gatt_action_context.value);

    debug_print("[pd_start_gatt_action] sent '%d' bytes to char '0x%04x' with "
                "res: %u\n",
                pd_gatt_action_context.value_length,
                pd_gatt_action_context.target_char->uuid16, res);

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);
}

void init_ble() {
    l2cap_init();

    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    att_server_init(NULL, NULL, NULL);

    gatt_client_init();

    hci_event_callback_registration.callback = &__pd_handle_hci_event;
    hci_add_event_handler(&hci_event_callback_registration);
}

void pd_bt_characteristics_discovery_loop() {
    static const uint32_t interval_ms = 1000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    if (pd_gatt_client_state != PD_GATT_CLIENT_STATE_CHARS_DISCOVERY) {
        // debug_print("[pd_bt_characteristics_discovery_loop] discovery "
        //             "completed returning.\n");
        return;
    }

    if (pd_gatt_warden_version_characteristic.uuid16 == 0) {
        pd_gatt_get_characteristic(PD_WARDEN_VERSION_GATT_CHAR_UUID16,
                                   &pd_gatt_warden_version_characteristic);

        return;
    }

    if (pd_gatt_warden_battery_voltage_characteristic.uuid16 == 0) {
        pd_gatt_get_characteristic(
            PD_WARDEN_BATTERY_VOLTAGE_GATT_CHAR_UUID16,
            &pd_gatt_warden_battery_voltage_characteristic);

        return;
    }

    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_READY);
};

// can be handle via repeating timers but let's do it this way
void pd_bt_queue_processing_loop() {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    if (pd_gatt_client_state == PD_GATT_CLIENT_STATE_CHARS_DISCOVERY) {
        debug_print("[pd_bt_queue_processing_loop] characteristics discovery "
                    "in progress returning.\n");
        return;
    }

    if (pd_is_queue_empty(&g_bt_functions_queue)) {
        debug_print("[pd_bt_queue_processing_loop] queue empty\n");
        return;
    }

    debug_print(
        "[pd_bt_queue_processing_loop] fired, pd_gatt_client_state: %d\n",
        pd_gatt_client_state);

    if (pd_gatt_client_state == PD_GATT_CLIENT_STATE_READY) {
        update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_PROCESSING);

        debug_print("[pd_bt_queue_processing_loop] processing function\n");

        pd_move_queue(&g_bt_functions_queue);
    }
};
