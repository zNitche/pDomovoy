#include "../../includes/led_blink.h"
#include <stdbool.h>

#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "../../includes/defines.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/time.h"

void blink_blocking(int pin, int times, int time_between) {
    bool state = true;

    for (int i = 0; i < times; i++) {
        gpio_put(pin, state);
        state = !state;

        sleep_ms(time_between);
    }

    gpio_put(pin, false);
}

void blink_status_untill_start(int32_t time_between,
                               repeating_timer_callback_t cb,
                               repeating_timer_t* timer, bool reclaim_on_init) {
    if (reclaim_on_init || !timer->alarm_id) {
        if (reclaim_on_init) {
            blink_status_untill_stop(timer);
        }

        add_repeating_timer_ms(time_between, cb, NULL, timer);
    }
}

void blink_status_untill_stop(repeating_timer_t* timer) {
    bool g = cancel_repeating_timer(timer);
    gpio_put(PDA_STATUS_LED_PIN, false);
}

void cyw34_blink_untill_start(int32_t time_between, repeating_timer_t* timer,
                              bool reclaim_on_init) {
    if (reclaim_on_init || !timer->alarm_id) {
        if (reclaim_on_init) {
            cyw34_blink_untill_stop(timer);
        }

        add_repeating_timer_ms(time_between, _blink_onboard_led_cb, NULL,
                               timer);
    }
}

void cyw34_blink_untill_stop(repeating_timer_t* timer) {
    bool g = cancel_repeating_timer(timer);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
}

void blink_onboard_led_blocking(int times, int time_between) {
    bool state = true;

    for (int i = 0; i < times; i++) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
        state = !state;

        sleep_ms(time_between);
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
}

bool _blink_onboard_led_cb(__unused repeating_timer_t* t) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    busy_wait_us(30000);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);

    return true;
}
