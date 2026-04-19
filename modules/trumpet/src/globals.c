#include "../includes/globals.h"
#include "pdomovoy_common/types.h"

// should be accessed only from core_0

bool g_btn_blocked = false;

repeating_timer_t g_onboard_led_blink_timer;
repeating_timer_t g_status_led_blink_timer;
