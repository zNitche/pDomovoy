#include "../../includes/bluetooth/globals.h"

BleServiceContext ble_service_context = {.is_connected = false};
btstack_packet_callback_registration_t hci_event_callback_registration;

PdGattCharacteristicDiscoveryContext pd_characteristic_discovery_context;

gatt_client_characteristic_t pd_gatt_warden_version_characteristic = {.uuid16 =
                                                                          0};
gatt_client_characteristic_t pd_gatt_warden_battery_voltage_characteristic = {
    .uuid16 = 0};
;

gatt_client_characteristic_t pd_gatt_warden_alarm_state_characteristic = {
    .uuid16 = 0};
;

PdGattActionContext pd_gatt_action_context = {};

enum GattClientStateEnum pd_gatt_client_state = PD_GATT_CLIENT_STATE_UNSET;
