#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

extern hci_con_handle_t ble_connection_handle;

extern btstack_packet_callback_registration_t hci_event_callback_registration;
extern int ble_notification_enabled;
