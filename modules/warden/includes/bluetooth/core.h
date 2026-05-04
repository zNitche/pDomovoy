#pragma once

#include "../../includes/bluetooth/types.h"
#include "btstack.h"
#include "pdomovoy_common/queue.h"
#include "pico/stdlib.h"
#include "pico/time.h"

int pd_bt_send_version_code();
int pd_bt_send_battery_voltage();
int pd_bt_send_alarm_state();

void pd_gatt_get_characteristic(uint16_t uuid16,
                                gatt_client_characteristic_t* characteristic);

void pd_bt_sub_for_characteristics_notifications(
    gatt_client_characteristic_t* characteristic);

void pd_start_gatt_action();

void init_ble();

void pd_bt_process_queue_prio_func(QueueFunction function);
void pd_bt_process_queue();
bool pd_bt_queue_processing_loop(repeating_timer_t* timer);
void pd_bt_characteristics_discovery_loop();

void pd_start_bt_queue_processing_loop_bg(repeating_timer_t* timer);
void pd_stop_bt_queue_processing_loop_bg(repeating_timer_t* timer);
