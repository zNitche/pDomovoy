#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>
#include "pdomovoy_common/types.h"
#include "pico_aht20/aht20.h"
#include "pico_aht20/defines.h"
#include "pico_ssd1306/ssd1306.h"

// should be accessed only from core_0

extern bool g_btn_blocked;
extern int g_current_page_id;

extern repeating_timer_t g_onboard_led_blink_timer;
extern repeating_timer_t g_status_led_blink_timer;

extern AHT20_I2C g_aht20_i2c;
extern SSD1306_I2C g_ssd1306_i2c;
