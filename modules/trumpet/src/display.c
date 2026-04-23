#include "../includes/display.h"

#include "pico/stdlib.h"

#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pico_ssd1306/ssd1306.h"

void extend_screen_display_time() {
    g_screen_enabled_time = to_ms_since_boot(get_absolute_time());
}

void toggle_screen(bool state) {
    if (state) {
        g_screen_enabled_time = to_ms_since_boot(get_absolute_time());

        ssd1306_clear(g_ssd1306_i2c);
        ssd1306_display_on(g_ssd1306_i2c);
    } else {
        ssd1306_display_off(g_ssd1306_i2c);
    }

    g_display_off = !state;
    g_navigation_blocked = !state;
}

bool _screen_saver_cb(__unused repeating_timer_t* t) {
    const uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (!g_screen_enabled_time) {
        g_screen_enabled_time = current_time;
    }

    if (g_display_off) {
        return true;
    }

    const uint32_t screen_active_since = current_time - g_screen_enabled_time;

    if (screen_active_since > PD_SCREEN_SAVER_AFTER) {
        toggle_screen(false);
    }

    return true;
}

void start_screen_save_timer() {
    add_repeating_timer_ms(1000, _screen_saver_cb, NULL, &g_screen_saver_timer);
}

void stop_screen_save_timer() { cancel_repeating_timer(&g_screen_saver_timer); }
