#include "../../includes/bluetooth/helpers.h"

#include "../../includes/bluetooth/globals.h"
#include "../../includes/bluetooth/types.h"
#include "pdomovoy_common/debug_print.h"

void update_pd_gatt_client_state(enum GattClientStateEnum state) {
    pd_gatt_client_state = state;
    debug_print("[PD_BLE] set pd_gatt_client_state to %d\n", state);
}