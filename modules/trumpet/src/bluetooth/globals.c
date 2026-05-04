#include "../../includes/bluetooth/globals.h"

#include "btstack.h"
#include "pico/stdlib.h"

hci_con_handle_t ble_connection_handle = HCI_CON_HANDLE_INVALID;

btstack_packet_callback_registration_t hci_event_callback_registration;
int ble_notification_enabled;
