#pragma once

#include "pico/stdlib.h"
#include "pico/time.h"

void pd_bt_send_version_code();

void init_ble();

bool pd_bt_queue_timer_cb(struct repeating_timer* t);
