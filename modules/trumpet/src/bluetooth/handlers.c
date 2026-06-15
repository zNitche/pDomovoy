#include "../../includes/bluetooth/handlers.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/helpers.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pd_common_config.h"
#include "pdomovoy_common/bluetooth.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "trumpet_core_gatt.h"
#include <stdio.h>

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
            bd_addr_t remote_addr;
            gap_subevent_le_connection_complete_get_peer_address(packet,
                                                                 remote_addr);

            bd_addr_t warden_address;
            hci_con_handle_t connection_handle =
                gap_subevent_le_connection_complete_get_connection_handle(
                    packet);

            parse_mac_address(warden_address, PD_WARDEN_BT_MAC);

            if (!are_mac_adresses_the_same(remote_addr, warden_address)) {
                gap_disconnect(connection_handle);

                debug_print(
                    "[GATT_SERVER] client's MAC mismatch, disconnecting\n");
                return;
            }

            gap_advertisements_enable(0);

            gap_update_connection_parameters(connection_handle, 80, 80, 0, 5000);

            debug_print("[GATT_SERVER] client connected \n");
            break;
        }
        break;

    case HCI_EVENT_CONNECTION_REQUEST:
        debug_print("[GATT_SERVER] conection request\n");
        break;

    case HCI_EVENT_DISCONNECTION_COMPLETE:
        ble_notification_enabled = 0;

        __pd_client_state_cleanup(false);

        if (g_alarm_state != ALARM_STATE_NONE) {
            g_alarm_state = ALARM_STATE_TRIGGERED;
        }

        gap_advertisements_enable(1);

        debug_print("[GATT_SERVER] disconnected\n");
        break;

    case ATT_EVENT_CAN_SEND_NOW:
        __pd_send_trumpets_alarm_state_notification();

        break;

    default:
        break;
    }
}

uint16_t __pd_att_read_callback(hci_con_handle_t connection_handle,
                                uint16_t att_handle, uint16_t offset,
                                uint8_t* buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);

    switch (att_handle) {
    case ATT_CHARACTERISTIC_00001104_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        static uint8_t alarm_state_buff[sizeof(int)];
        int alarm_state = g_alarm_state;

        memcpy(alarm_state_buff, &alarm_state, sizeof(alarm_state_buff));

        return att_read_callback_handle_blob(&alarm_state_buff,
                                             sizeof(alarm_state_buff), offset,
                                             buffer, buffer_size);

    default:
        break;
    }

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
        memcpy(&g_warden_remote_data.software_version, buffer,
               sizeof(g_warden_remote_data.software_version));

        g_warden_remote_data.connected = true;
        g_warden_remote_data.connected_timestamp =
            to_ms_since_boot(get_absolute_time());

        debug_print("[GATT_SERVER] got warden's version: %s\n",
                    g_warden_remote_data.software_version);

        break;

    case ATT_CHARACTERISTIC_00001102_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        float voltage_float;
        memcpy(&voltage_float, buffer, sizeof(float));

        debug_print("[GATT_SERVER] got warden's battery voltage: %.2f\n",
                    voltage_float);

        g_warden_remote_data.battery_voltage = voltage_float;

        break;

    case ATT_CHARACTERISTIC_00001103_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE:
        int alarm_state;
        memcpy(&alarm_state, buffer, sizeof(int));

        g_alarm_state = alarm_state;

        if (g_alarm_state == ALARM_STATE_NONE) {
            g_alarm_armed_timestamp = 0;
        }

        if (g_alarm_state == ALARM_STATE_STANDBY_INIT ||
            g_alarm_state == ALARM_STATE_STANDBY) {
            if (g_alarm_armed_timestamp == 0) {
                g_alarm_armed_timestamp = to_ms_since_boot(get_absolute_time());
            }
        }

        debug_print("[GATT_SERVER] got warden's alarm_state: %d\n",
                    alarm_state);
        break;

    case ATT_CHARACTERISTIC_00001104_0000_1000_8000_00805F9B34FB_01_CLIENT_CONFIGURATION_HANDLE:
        ble_notification_enabled =
            little_endian_read_16(buffer, 0) ==
            GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;

        ble_connection_handle = connection_handle;

        if (ble_notification_enabled) {
            att_server_request_can_send_now_event(ble_connection_handle);
        }

        debug_print("[GATT_SERVER] got trumpets's alarm_state client "
                    "configuration handle\n");
        break;

    default:
        debug_print("[GATT_SERVER][att_write_callback] att_handle: 0x%04x\n",
                    att_handle);
        break;
    }

    return 0;
}

void __pd_send_trumpets_alarm_state_notification() {
    static uint8_t alarm_state_buff[sizeof(int)];
    int alarm_state = g_alarm_state;

    memcpy(alarm_state_buff, &alarm_state, sizeof(alarm_state_buff));

    att_server_notify(
        ble_connection_handle,
        ATT_CHARACTERISTIC_00001104_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE,
        &alarm_state_buff, sizeof(alarm_state_buff));

    debug_print(
        "[GATT_SERVER] sent notification for trumpet's alarm state: %d\n",
        alarm_state);
}
