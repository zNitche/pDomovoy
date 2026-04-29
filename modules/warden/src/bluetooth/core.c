#include "../../includes/bluetooth/core.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/handlers.h"
#include "../../includes/bluetooth/delegates.h"
#include "../../includes/bluetooth/helpers.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pd_common_config.h"
#include "pdomovoy_common/bluetooth.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/defines.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"

void pd_bt_send_version_code() {
    update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_GET_CHAR);

    gatt_client_discover_characteristics_for_service_by_uuid16(
        __pd_handle_gatt_client_event, ble_service_context.connection_handle,
        &ble_service_context.service, PD_WARDEN_VERSION_GATT_VALUE_HANDLE);
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

bool pd_bt_queue_timer_cb(struct repeating_timer* t) {
    debug_print("pd_bt_queue_timer_cb fired\n");

    if (pd_gatt_client_state == PD_GATT_CLIENT_STATE_READY) {
        update_pd_gatt_client_state(PD_GATT_CLIENT_STATE_PROCESSING);

        pd_move_queue(&g_bt_functions_queue);
    }

    return true;
};
