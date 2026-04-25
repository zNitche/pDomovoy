#include "../includes/bluetooth.h"

#include "../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void hci_event_handler(uint8_t packet_type, uint16_t channel,
                              uint8_t* packet, uint16_t size) {
    UNUSED(size);
    UNUSED(channel);
    bd_addr_t local_addr;

    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint8_t event_type = hci_event_packet_get_type(packet);

    switch (event_type) {
    case BTSTACK_EVENT_STATE:
        if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
            gap_local_bd_addr(local_addr);
            debug_print("[BTstack] up and running on %s.\n",
                        bd_addr_to_str(local_addr));

            // ble_connect_to_server();
            gap_set_scan_params(1, 0x0030, 0x0030, 0);
            gap_start_scan();
        }
        break;
    case GAP_EVENT_ADVERTISING_REPORT: {
        bd_addr_t address;
        gap_event_advertising_report_get_address(packet, address);
        uint8_t event_type =
            gap_event_advertising_report_get_advertising_event_type(packet);
        int8_t rssi = gap_event_advertising_report_get_rssi(packet);
        uint8_t length = gap_event_advertising_report_get_data_length(packet);

        debug_print("Device Found: %s | RSSI: %d | Type: %d\n",
                    bd_addr_to_str(address), rssi, event_type);
        break;
    }
    case HCI_EVENT_LE_META:
        // wait for connection complete
        switch (hci_event_le_meta_get_subevent_code(packet)) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            printf("Connected\n");
            break;
        default:
            break;
        }
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        printf("Disconnected\n");
        break;
    default:
        break;
    }
}

void init_ble() {
    l2cap_init();

    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    // setup empty ATT server - only needed if LE Peripheral does ATT queries on
    // its own, e.g. Android and iOS
    att_server_init(NULL, NULL, NULL);

    gatt_client_init();

    hci_event_callback_registration.callback = &hci_event_handler;
    hci_add_event_handler(&hci_event_callback_registration);
}

void turn_ble_on() { hci_power_control(HCI_POWER_ON); }

void ble_connect_to_server() {
    bd_addr_t remote_addr = {};
    bd_addr_type_t remote_type = BD_ADDR_TYPE_LE_RANDOM;

    const uint8_t status = gap_connect(remote_addr, remote_type);

    debug_print("ble connection status: %d\n", status);
}
