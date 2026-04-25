#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>
#include "pdomovoy_common/types.h"
#include "pico_aht20/aht20.h"
#include "pico_aht20/defines.h"
#include "pico_ssd1306/ssd1306.h"

// should be accessed only from core_0

extern bool g_navigation_blocked;
extern bool g_alarm_in_standby;
extern bool g_display_off;

extern uint32_t g_screen_enabled_time;
extern int g_current_page_id;

extern repeating_timer_t g_onboard_led_blink_timer;
extern repeating_timer_t g_screen_saver_timer;

extern AHT20_I2C g_aht20_i2c;
extern SSD1306_I2C g_ssd1306_i2c;

extern float g_battery_voltage;
extern repeating_timer_t g_adc_voltmeter_timer;

extern uint8_t const profile_data[];
