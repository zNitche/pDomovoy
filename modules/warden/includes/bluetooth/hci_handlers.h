#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

void __handle_btstack_event_state(uint8_t* packet);
void __handle_gap_event_advertising_report(uint8_t* packet);
void __handle_hci_event_le_meta(uint8_t* packet);
void __handle_hci_event_disconnection_complete(uint8_t* packet);
