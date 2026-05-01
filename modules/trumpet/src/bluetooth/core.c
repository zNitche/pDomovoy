#include "../../includes/bluetooth/core.h"

#include "../../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "trumpet_core_gatt.h"
#include <stdio.h>

static btstack_packet_callback_registration_t hci_event_callback_registration;

void __setup_gap_advertisements() {
    uint8_t adv_data[] = {0x02,
                          BLUETOOTH_DATA_TYPE_FLAGS,
                          0x06,
                          0x17,
                          BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
                          'T',
                          'R',
                          'U',
                          'M',
                          'P',
                          'E',
                          'T'};

    uint16_t adv_int_min = 800;
    uint16_t adv_int_max = 800;
    uint8_t adv_type = 0;

    bd_addr_t null_addr;
    memset(null_addr, 0, 6);

    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t*)adv_data);
    gap_advertisements_enable(1);
}

static void __packet_handler(uint8_t packet_type, uint16_t channel,
                             uint8_t* packet, uint16_t size) {
    UNUSED(size);
    UNUSED(channel);
    bd_addr_t local_addr;

    if (packet_type != HCI_EVENT_PACKET)
        return;

    uint8_t event_type = hci_event_packet_get_type(packet);

    switch (event_type) {
    case BTSTACK_EVENT_STATE:
        if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING)
            return;

        gap_local_bd_addr(local_addr);
        debug_print("[BTstack] up and running on %s.\n",
                    bd_addr_to_str(local_addr));

        __setup_gap_advertisements();

        break;
    case HCI_EVENT_LE_META:
        switch (hci_event_le_meta_get_subevent_code(packet)) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            gap_advertisements_enable(0);
            debug_print("client connected \n");
            break;
        }
        break;

    case HCI_EVENT_CONNECTION_REQUEST:
        debug_print("conection request\n");
        break;

    case HCI_EVENT_CONNECTION_COMPLETE:
        debug_print("connected\n");
        break;

    case HCI_EVENT_DISCONNECTION_COMPLETE:
        gap_advertisements_enable(1);

        g_warden_battery_voltage = 0.0;

        debug_print("disconnected\n");
        break;

    default:
        break;
    }
}

static uint16_t __att_read_callback(hci_con_handle_t connection_handle,
                                    uint16_t att_handle, uint16_t offset,
                                    uint8_t* buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);

    return 0;
}

static int __att_write_callback(hci_con_handle_t connection_handle,
                                uint16_t att_handle, uint16_t transaction_mode,
                                uint16_t offset, uint8_t* buffer,
                                uint16_t buffer_size) {
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(buffer_size);

    switch (att_handle) {
    case ATT_CHARACTERISTIC_00001101_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        debug_print("got warden's version: %s\n", buffer);
        break;

    case ATT_CHARACTERISTIC_00001102_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        float voltage_float;
        memcpy(&voltage_float, buffer, sizeof(float));

        debug_print("got warden's battery voltage: %.2f\n", voltage_float);

        g_warden_battery_voltage = voltage_float;

        break;

    default:
        break;
    }

    return 0;
}

void init_ble() {
    l2cap_init();
    sm_init();

    gatt_client_set_required_security_level(LEVEL_2);
    sm_set_authentication_requirements(SM_AUTHREQ_BONDING |
                                       SM_AUTHREQ_SECURE_CONNECTION);

    att_server_init(profile_data, __att_read_callback, __att_write_callback);

    hci_event_callback_registration.callback = &__packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    att_server_register_packet_handler(__packet_handler);
}
