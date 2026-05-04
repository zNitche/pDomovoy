#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

void __pd_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                         uint16_t size);

uint16_t __pd_att_read_callback(hci_con_handle_t connection_handle,
                                uint16_t att_handle, uint16_t offset,
                                uint8_t* buffer, uint16_t buffer_size);

int __pd_att_write_callback(hci_con_handle_t connection_handle,
                            uint16_t att_handle, uint16_t transaction_mode,
                            uint16_t offset, uint8_t* buffer,
                            uint16_t buffer_size);

void __pd_send_trumpets_alarm_state_notification();
