#pragma once

#include <stdbool.h>

#include "btstack.h"
#include "pico/stdlib.h"

typedef struct {
    hci_con_handle_t connection_handle;
    gatt_client_service_t service;
    gatt_client_characteristic_t characteristic;
    bool is_connected;
} BleServiceContext;

enum GattClientStateEnum {
    PD_GATT_CLIENT_STATE_UNSET,
    PD_GATT_CLIENT_STATE_READY,
    PD_GATT_CLIENT_STATE_GET_WARDEN_VERSION_CHAR,
    PD_GATT_CLIENT_STATE_READY_TO_WRITE_WARDEN_VERSION_CHAR,
};
