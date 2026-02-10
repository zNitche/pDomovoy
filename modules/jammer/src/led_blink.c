#include "../includes/led_blink.h"
#include <stdbool.h>

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