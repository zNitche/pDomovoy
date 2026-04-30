#pragma once

#include "pico/stdlib.h"

int pd_bt_send_version_code();
void pd_start_gatt_action();

void init_ble();

void pd_bt_queue_processing_loop();
