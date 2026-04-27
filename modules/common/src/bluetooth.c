#include "pdomovoy_common/bluetooth.h"

#include "btstack.h"
#include "pico/stdlib.h"

void parse_mac_address(bd_addr_t target, unsigned int source[6]) {
    for (int i = 0; i < 6; i++) {
        target[i] = (uint8_t)source[i];
    }
}

void turn_bluetooth_on() { hci_power_control(HCI_POWER_ON); }
