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
#include "pdomovoy_common/version.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

int pd_bt_send_version_code() {
    pd_gatt_action_context.char_uuid16 = PD_WARDEN_VERSION_GATT_VALUE_HANDLE;
    pd_gatt_action_context.value = WARDEN_VERSION;
    pd_gatt_action_context.value_length = sizeof(WARDEN_VERSION);

    pd_start_gatt_action();

    return 0;
}

void pd_start_gatt_action() {
    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_GET_CHAR);

    gatt_client_discover_characteristics_for_service_by_uuid16(
        __pd_handle_gatt_client_event, ble_service_context.connection_handle,
        &ble_service_context.service, pd_gatt_action_context.char_uuid16);
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

void pd_bt_queue_processing_loop() {
    static const uint32_t interval_ms = 5000;
    static uint32_t next_runtime = 0;

    const uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time < next_runtime) {
        return;
    }

    next_runtime = current_time + interval_ms;

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
