#pragma once

#include <stdbool.h>

#include "btstack.h"
#include "pico/stdlib.h"

typedef struct {
    hci_con_handle_t connection_handle;
    gatt_client_service_t service;
    bool is_connected;
} BleServiceContext;

typedef struct {
    gatt_client_characteristic_t target_char;
    uint16_t char_uuid16;
    uint8_t* value;
    uint16_t value_length;
} PdGattActionContext;

enum GattClientStateEnum {
    PD_GATT_CLIENT_STATE_UNSET,
    PD_GATT_CLIENT_STATE_READY,
    PD_GATT_CLIENT_STATE_PROCESSING,
    PD_GATT_CLIENT_STATE_GET_CHAR,
    PD_GATT_CLIENT_STATE_READY_TO_PROCESS_CHAR,
};
