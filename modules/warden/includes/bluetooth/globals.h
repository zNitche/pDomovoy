#pragma once

#include "./types.h"
#include "btstack.h"
#include "pico/stdlib.h"

extern BleServiceContext ble_service_context;
extern btstack_packet_callback_registration_t hci_event_callback_registration;

extern PdGattActionContext pd_gatt_action_context;

extern enum GattClientStateEnum pd_gatt_client_state;