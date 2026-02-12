#include "../../includes/button.h"

#include "../../includes/types.h"
#include <stdbool.h>

#include "pico/time.h"

void init_button_irq(uint gpio, button_callback callback) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_down(gpio);
    gpio_set_irq_enabled_with_callback(
        gpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, callback);
}

bool debounce_push_button(uint32_t event, int min_delay_between_clicks) {
    static bool is_pressed = false;
    static uint32_t last_call = 0;

    const uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (event == GPIO_IRQ_EDGE_FALL) {
        is_pressed = false;
        last_call = current_time;

        return false;
    }

    if (current_time - last_call < min_delay_between_clicks) {
        return false;
    }

    if (event == GPIO_IRQ_EDGE_RISE && is_pressed) {
        return false;
    }

    is_pressed = true;
    last_call = current_time;

    return true;
}

void count_clicks_in_row(uint32_t* last_click_time, int* clicks_in_row,
                         int min_time_between_clicks) {
    const uint32_t current_click_time = to_ms_since_boot(get_absolute_time());

    if (current_click_time - *last_click_time <= min_time_between_clicks) {
        *clicks_in_row += 1;
    } else {
        *clicks_in_row = 0;
    }

    *last_click_time = current_click_time;
}