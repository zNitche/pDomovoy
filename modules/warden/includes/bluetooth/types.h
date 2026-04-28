#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

typedef struct {
    hci_con_handle_t connection_handle;
    gatt_client_service_t service;
    gatt_client_characteristic_t characteristic;
} BleServiceContext;

enum GattClientStateEnum {
    PD_GATT_CLIENT_STATE_UNSET,
    PD_GATT_CLIENT_STATE_CONFIGURED,
};
