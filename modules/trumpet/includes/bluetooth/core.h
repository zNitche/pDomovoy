#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

void init_ble();

void __setup_gap_advertisements();
static void __packet_handler(uint8_t packet_type, uint16_t channel,
                             uint8_t* packet, uint16_t size);
static uint16_t __att_read_callback(hci_con_handle_t connection_handle,
                                    uint16_t att_handle, uint16_t offset,
                                    uint8_t* buffer, uint16_t buffer_size);
static int __att_write_callback(hci_con_handle_t connection_handle,
                                uint16_t att_handle, uint16_t transaction_mode,
                                uint16_t offset, uint8_t* buffer,
                                uint16_t buffer_size);
