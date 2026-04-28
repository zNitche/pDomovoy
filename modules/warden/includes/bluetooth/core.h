#pragma once

#include "pico/stdlib.h"

void handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                              uint8_t* packet, uint16_t size);
void handle_hci_event(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                      uint16_t size);
void init_ble();
