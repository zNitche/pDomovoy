#include "../../includes/bluetooth/hci_handlers.h"

#include "../../includes/bluetooth/core.h"
#include "../../includes/bluetooth/globals.h"
#include "btstack.h"
#include "pd_common_config.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/defines.h"
#include "pico/stdlib.h"

void __handle_btstack_event_state(uint8_t* packet) {
    bd_addr_t local_addr;

    if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
        gap_local_bd_addr(local_addr);

        debug_print("[BTstack] up and running on %s.\n",
                    bd_addr_to_str(local_addr));

        gap_set_scan_params(1, 0x0030, 0x0030, 0);
        gap_start_scan();
    }
}

void __handle_gap_event_advertising_report(uint8_t* packet) {
    bd_addr_t address;
    gap_event_advertising_report_get_address(packet, address);

    bd_addr_t server_address;
    parse_mac_address(server_address, PD_SERVER_BT_MAC);

    if (memcmp(address, server_address, 6) == 0) {
        debug_print("Target found! Connecting...\n");

        gap_stop_scan();
        bd_addr_type_t server_addr_type =
            gap_event_advertising_report_get_address_type(packet);

        gap_connect(server_address, server_addr_type);
    }
}

void __handle_hci_event_le_meta(uint8_t* packet) {
    switch (hci_event_le_meta_get_subevent_code(packet)) {

    case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
        ble_service_context.connection_handle =
            gap_subevent_le_connection_complete_get_connection_handle(packet);

        debug_print("Connected!\n");

        gatt_client_discover_primary_services(
            handle_gatt_client_event, ble_service_context.connection_handle);

        break;

    default:
        break;
    }
}

void __handle_hci_event_disconnection_complete(uint8_t* packet) {
    debug_print("Disconnected\n");

    ble_service_context.connection_handle = HCI_CON_HANDLE_INVALID;
    gap_start_scan();
}
