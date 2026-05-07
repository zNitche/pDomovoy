#pragma once

#include "btstack.h"
#include "pico/stdlib.h"
#include "stdbool.h"

void parse_mac_address(bd_addr_t target, unsigned int source[6]);
void turn_bluetooth_on();
bool are_mac_adresses_the_same(bd_addr_t addr1, bd_addr_t addr2);
