#include "../../includes/bluetooth/core.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/handlers.h"
#include "../../includes/globals.h"
#include "btstack.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

void init_ble() {
    l2cap_init();
    sm_init();

    gatt_client_set_required_security_level(LEVEL_2);
    sm_set_authentication_requirements(SM_AUTHREQ_BONDING |
                                       SM_AUTHREQ_SECURE_CONNECTION);

    att_server_init(profile_data, __pd_att_read_callback,
                    __pd_att_write_callback);

    hci_event_callback_registration.callback = &__pd_packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    att_server_register_packet_handler(__pd_packet_handler);
}
