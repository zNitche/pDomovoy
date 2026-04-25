#include "../includes/bluetooth.h"

#include "../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;

const bd_addr_t server_remote_mac = {};

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

        if (memcmp(address, server_remote_mac, 6) == 0) {
            debug_print("Target found! Connecting...\n");

            gap_stop_scan();
            bd_addr_type_t server_addr_type =
                gap_event_advertising_report_get_address_type(packet);

            gap_connect(server_remote_mac, server_addr_type);
        }
        break;
    }
    case HCI_EVENT_LE_META:
        switch (hci_event_le_meta_get_subevent_code(packet)) {

        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            // Check the status byte (usually at index 3 for Meta events)
            uint8_t status = packet[3];
            if (status == 0) {
                debug_print("Connected!\n");
            } else {
                debug_print("Connection failed with status: 0x%02x\n", status);
            }

            break;

        default:
            break;
        }

        break;
    case HCI_EVENT_COMMAND_COMPLETE:
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        debug_print("Disconnected. Status: 0x%02x\n", packet[2]);

        gap_start_scan();
        break;
    default:
        break;
    }
}

void init_ble() {
    l2cap_init();

    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    att_server_init(NULL, NULL, NULL);

    gatt_client_init();

    hci_event_callback_registration.callback = &hci_event_handler;
    hci_add_event_handler(&hci_event_callback_registration);
}

void turn_ble_on() { hci_power_control(HCI_POWER_ON); }
