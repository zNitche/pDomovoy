#include "../../includes/bluetooth/core.h"

#include "../../includes/bluetooth/client_handlers.h"
#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/hci_handlers.h"
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

void __pd_handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                                   uint8_t* packet, uint16_t size) {
    UNUSED(packet_type);
    UNUSED(channel);
    UNUSED(size);

    uint8_t att_status;
    uint8_t event_type = hci_event_packet_get_type(packet);

    switch (event_type) {
    case GATT_EVENT_SERVICE_QUERY_RESULT:
        __handle_gatt_event_service_query_result(packet);
        break;

    case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
        __handle_gatt_event_characteristic_query_result(packet);
        break;

    case GATT_EVENT_QUERY_COMPLETE:
        __handle_gatt_event_query_complete(packet);
        break;

    default:
        break;
    }
}

void __pd_handle_hci_event(uint8_t packet_type, uint16_t channel,
                           uint8_t* packet, uint16_t size) {
    UNUSED(size);
    UNUSED(channel);

    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint8_t event_type = hci_event_packet_get_type(packet);

    switch (event_type) {
    case BTSTACK_EVENT_STATE:
        __handle_btstack_event_state(packet);
        break;

    case GAP_EVENT_ADVERTISING_REPORT:
        __handle_gap_event_advertising_report(packet);
        break;

    case HCI_EVENT_LE_META:
        __handle_hci_event_le_meta(packet);
        break;

    case HCI_EVENT_COMMAND_COMPLETE:
        break;

    case HCI_EVENT_DISCONNECTION_COMPLETE:
        __handle_hci_event_disconnection_complete(packet);
        break;

    default:
        break;
    }
}

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
