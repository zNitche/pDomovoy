#include "../../includes/bluetooth/handlers.h"

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
