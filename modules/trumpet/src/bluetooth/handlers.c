#include "../../includes/bluetooth/handlers.h"

#include "../../includes/bluetooth/helpers.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/stdlib.h"
#include "trumpet_core_gatt.h"

void __pd_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                         uint16_t size) {
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
        debug_print("[GATT_SERVER] up and running on %s.\n",
                    bd_addr_to_str(local_addr));

        __pd_setup_gap_advertisements();

        break;
    case HCI_EVENT_LE_META:
        switch (hci_event_le_meta_get_subevent_code(packet)) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
            gap_advertisements_enable(0);
            debug_print("[GATT_SERVER] client connected \n");
            break;
        }
        break;

    case HCI_EVENT_CONNECTION_REQUEST:
        debug_print("[GATT_SERVER] conection request\n");
        break;

    case HCI_EVENT_CONNECTION_COMPLETE:
        debug_print("[GATT_SERVER] connected\n");
        break;

    case HCI_EVENT_DISCONNECTION_COMPLETE:
        gap_advertisements_enable(1);

        __pd_client_state_cleanup();

        debug_print("[GATT_SERVER] disconnected\n");
        break;

    default:
        break;
    }
}

uint16_t __pd_att_read_callback(hci_con_handle_t connection_handle,
                                uint16_t att_handle, uint16_t offset,
                                uint8_t* buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);

    return 0;
}

int __pd_att_write_callback(hci_con_handle_t connection_handle,
                            uint16_t att_handle, uint16_t transaction_mode,
                            uint16_t offset, uint8_t* buffer,
                            uint16_t buffer_size) {
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(buffer_size);

    switch (att_handle) {
    case ATT_CHARACTERISTIC_00001101_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        g_warden_connected = true;
        memcpy(&g_warden_version, buffer, sizeof(g_warden_version));

        debug_print("[GATT_SERVER] got warden's version: %s\n",
                    g_warden_version);

        break;

    case ATT_CHARACTERISTIC_00001102_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        float voltage_float;
        memcpy(&voltage_float, buffer, sizeof(float));

        debug_print("[GATT_SERVER] got warden's battery voltage: %.2f\n",
                    voltage_float);

        g_warden_battery_voltage = voltage_float;

        break;

    case ATT_CHARACTERISTIC_00001103_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        int alarm_state;
        memcpy(&alarm_state, buffer, sizeof(int));

        debug_print("[GATT_SERVER] got warden's alarm_state: %d\n",
                    alarm_state);
        break;

    default:
        break;
    }

    return 0;
}
