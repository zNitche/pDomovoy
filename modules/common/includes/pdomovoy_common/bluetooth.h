#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

void parse_mac_address(bd_addr_t target, unsigned int source[6]);
void turn_bluetooth_on();
