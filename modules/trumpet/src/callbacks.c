#include "../includes/callbacks.h"

#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"

void gpio_irq_callback(uint gpio, uint32_t event) {
    switch (gpio) {
    case PD_PREVIOUS_BUTTON_PIN:
        previous_button_callback(event);
        break;

    case PD_APPLY_BUTTON_PIN:
        apply_button_callback(event);
        break;

    case PD_NEXT_BUTTON_PIN:
        next_button_callback(event);
        break;

    default:
        break;
    }
}

void previous_button_callback(uint32_t event) {
    static ButtonDebounceCtx button_debounce_ctx = {.is_pressed = false,
                                                    .last_call = 0};

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME,
                              &button_debounce_ctx)) {
        return;
    }

    if (g_btn_blocked) {
        return;
    }

    debug_print("previous button click\n");
}

void apply_button_callback(uint32_t event) {
    static ButtonDebounceCtx button_debounce_ctx = {.is_pressed = false,
                                                    .last_call = 0};

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME,
                              &button_debounce_ctx)) {
        return;
    }

    if (g_btn_blocked) {
        return;
    }

    debug_print("apply button click\n");
}

void next_button_callback(uint32_t event) {
    static ButtonDebounceCtx button_debounce_ctx = {.is_pressed = false,
                                                    .last_call = 0};

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME,
                              &button_debounce_ctx)) {
        return;
    }

    if (g_btn_blocked) {
        return;
    }

    debug_print("next button click\n");
}
