#include "../includes/bluetooth.h"

#include "../includes/globals.h"
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "pico/stdlib.h"


void init_ble() {
    l2cap_init();
    sm_init();

    att_server_init(g_btstack_profile_data, NULL, NULL);

    gatt_client_set_required_security_level(LEVEL_2);
    sm_set_authentication_requirements(SM_AUTHREQ_BONDING | SM_AUTHREQ_SECURE_CONNECTION);
}

void turn_ble_on() {
    hci_power_control(HCI_POWER_ON);
}
