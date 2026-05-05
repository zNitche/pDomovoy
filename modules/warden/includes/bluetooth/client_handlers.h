#pragma once

#include "btstack.h"
#include "pico/stdlib.h"

void __handle_gatt_event_service_query_result(uint8_t* packet);
void __handle_gatt_event_characteristic_query_result(uint8_t* packet);
void __handle_gatt_event_query_complete(uint8_t* packet);
void __handle_gatt_notification_event(uint8_t* packet);

void __handle_alarm_state_gatt_notification(uint8_t* packet);
