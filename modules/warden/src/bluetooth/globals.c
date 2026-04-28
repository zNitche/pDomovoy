#include "../../includes/bluetooth/globals.h"

BleServiceContext ble_service_context = {};
btstack_packet_callback_registration_t hci_event_callback_registration;

enum GattClientStateEnum pd_gatt_client_state = PD_GATT_CLIENT_STATE_UNSET;
