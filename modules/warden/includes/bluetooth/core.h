#pragma once

#include "pico/stdlib.h"
#include "../../includes/bluetooth/types.h"
#include "btstack.h"

int pd_bt_send_version_code();
int pd_bt_send_battery_voltage();

void pd_gatt_get_characteristic(uint16_t uuid16,
                                gatt_client_characteristic_t* characteristic);

void pd_start_gatt_action();

void init_ble();

void pd_bt_queue_processing_loop();
void pd_bt_characteristics_discovery_loop();
