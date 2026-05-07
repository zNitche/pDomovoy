#include "pdomovoy_common/bluetooth.h"

#include "btstack.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

void parse_mac_address(bd_addr_t target, unsigned int source[6]) {
    for (int i = 0; i < 6; i++) {
        target[i] = (uint8_t)source[i];
    }
}

void turn_bluetooth_on() { hci_power_control(HCI_POWER_ON); }

bool are_mac_adresses_the_same(bd_addr_t addr1, bd_addr_t addr2) {
    if (memcmp(addr1, addr2, 6) != 0) {
        return false;
    }

    return true;
}
