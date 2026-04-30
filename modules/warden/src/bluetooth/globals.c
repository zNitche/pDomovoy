#include "../../includes/bluetooth/globals.h"

BleServiceContext ble_service_context = {.is_connected = false};
btstack_packet_callback_registration_t hci_event_callback_registration;

PdGattActionContext pd_gatt_action_context = {};

enum GattClientStateEnum pd_gatt_client_state = PD_GATT_CLIENT_STATE_UNSET;
