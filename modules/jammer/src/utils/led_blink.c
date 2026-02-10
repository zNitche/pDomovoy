#include "../../includes/led_blink.h"
#include <stdbool.h>

#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"

void blink_blocking(int pin, int times, int time_between) {
    bool state = true;

    for (int i = 0; i < times; i++) {
        gpio_put(pin, state);
        state = !state;

        sleep_ms(time_between);
    }

    gpio_put(pin, false);
}

void blink_untill_start(int pin, int32_t time_between,
                        repeating_timer_callback_t cb) {
    add_repeating_timer_ms(time_between, cb, NULL, &g_status_led_blink_timer);
}

void blink_untill_stop(int pin) {
    bool g = cancel_repeating_timer(&g_status_led_blink_timer);
    gpio_put(pin, false);
}