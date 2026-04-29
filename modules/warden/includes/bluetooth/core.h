#pragma once

#include "pico/stdlib.h"

void __pd_handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                                   uint8_t* packet, uint16_t size);
void __pd_handle_hci_event(uint8_t packet_type, uint16_t channel,
                                uint8_t* packet, uint16_t size);

void pd_bt_send_version_code();

void init_ble();
