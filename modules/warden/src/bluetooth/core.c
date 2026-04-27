#include "../../includes/bluetooth/core.h"

#include "../../includes/globals.h"
#include "btstack.h"
#include "pd_common_config.h"
#include "pdomovoy_common/bluetooth.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/version.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;
static hci_con_handle_t connection_handle;

static bool server_version_characteristic_set = false;

static gatt_client_service_t server_service;
static gatt_client_characteristic_t server_version_characteristic;

static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                                     uint8_t* packet, uint16_t size) {
    UNUSED(packet_type);
    UNUSED(channel);
    UNUSED(size);

    uint8_t att_status;
    uint8_t event_type = hci_event_packet_get_type(packet);

    switch (event_type) {
    case GATT_EVENT_SERVICE_QUERY_RESULT:
        gatt_event_service_query_result_get_service(packet, &server_service);
        // debug_print("storing service: uuid16 %04X\n", server_service.uuid16);

        debug_print("found service with uuid16: %d\n", server_service.uuid16);

        if (server_service.uuid16 != 0x1100) {
            return;
        }

        debug_print("storing service: uuid16 %d, start: %d, end: %d\n",
                    server_service.uuid16, server_service.start_group_handle,
                    server_service.end_group_handle);

        // gatt_client_discover_characteristics_for_service_by_uuid16(
        //     handle_gatt_client_event, connection_handle, &server_service,
        //     0xFF10);

        break;
    case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
        gatt_event_characteristic_query_result_get_characteristic(
            packet, &server_version_characteristic);

        // debug_print("storing characteristic: uuid16 0x%04X\n",
        //             server_version_characteristic.uuid16);

        debug_print("storing characteristic: uuid16 %d\n",
                    server_version_characteristic.uuid16);

        server_version_characteristic_set = true;

        break;
    case GATT_EVENT_QUERY_COMPLETE:
        if (!server_version_characteristic_set) {
            const uint8_t res =
                gatt_client_discover_characteristics_for_service_by_uuid16(
                    handle_gatt_client_event, connection_handle,
                    &server_service, 0x1101);
        }

        debug_print("query complete\n");
        break;
    default:
        break;
    }
}

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

            gap_set_scan_params(1, 0x0030, 0x0030, 0);
            gap_start_scan();
        }
        break;
    case GAP_EVENT_ADVERTISING_REPORT: {
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
        break;
    }
    case HCI_EVENT_LE_META:
        switch (hci_event_le_meta_get_subevent_code(packet)) {

        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            connection_handle =
                gap_subevent_le_connection_complete_get_connection_handle(
                    packet);

            debug_print("Connected!\n");

            gatt_client_discover_primary_services(handle_gatt_client_event,
                                                  connection_handle);

            // gatt_client_discover_primary_services_by_uuid16(
            //     handle_gatt_client_event, connection_handle, 4353);

            // const uint8_t res =
            //     gatt_client_write_value_of_characteristic_without_response(
            //         connection_handle, 0x0009, sizeof(WARDEN_VERSION),
            //         WARDEN_VERSION);

            // debug_print("sent warden version with res: %d\n", res);

            break;

        default:
            break;
        }

        break;
    case HCI_EVENT_COMMAND_COMPLETE:
        break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
        debug_print("Disconnected\n");

        connection_handle = HCI_CON_HANDLE_INVALID;
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
