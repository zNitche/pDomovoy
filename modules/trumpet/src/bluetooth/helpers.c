#include "../../includes/bluetooth/helpers.h"

#include "../../includes/globals.h"
#include "btstack.h"
#include "pico/stdlib.h"
#include <string.h>

void __pd_setup_gap_advertisements() {
    uint8_t adv_data[] = {0x02,
                          BLUETOOTH_DATA_TYPE_FLAGS,
                          0x06,
                          0x17,
                          BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
                          'T',
                          'R',
                          'U',
                          'M',
                          'P',
                          'E',
                          'T'};

    uint16_t adv_int_min = 800;
    uint16_t adv_int_max = 800;
    uint8_t adv_type = 0;

    bd_addr_t null_addr;
    memset(null_addr, 0, 6);

    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                  null_addr, 0x07, 0x00);
    gap_advertisements_set_data(sizeof(adv_data), (uint8_t*)adv_data);
    gap_advertisements_enable(1);
}

void __pd_client_state_cleanup() {
    g_warden_connected = false;

    g_warden_battery_voltage = 0;
    g_warden_alarm_state = ALARM_STATE_NONE;
    memset(g_warden_version, 0, 6);
}