#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>
#include "pdomovoy_common/types.h"

// should be accessed only from core_0

extern bool g_btn_blocked;

extern repeating_timer_t g_onboard_led_blink_timer;
extern repeating_timer_t g_status_led_blink_timer;
