#include "../includes/globals.h"
#include "pdomovoy_common/types.h"
#include "pico_aht20/aht20.h"
#include "pico_aht20/defines.h"
#include "pico_ssd1306/ssd1306.h"

// should be accessed only from core_0

bool g_alarm_in_standby = false;
bool g_navigation_blocked = false;
bool g_display_off = false;

uint32_t g_screen_enabled_time = 0;
int g_current_page_id = 0;

repeating_timer_t g_onboard_led_blink_timer;
repeating_timer_t g_screen_saver_timer;

AHT20_I2C g_aht20_i2c;
SSD1306_I2C g_ssd1306_i2c;
