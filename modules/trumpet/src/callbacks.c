#include "../includes/callbacks.h"

#include "../includes/bluetooth/helpers.h"
#include "../includes/defines.h"
#include "../includes/display.h"
#include "../includes/globals.h"
#include "../includes/pages.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/types.h"

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

    if (g_navigation_blocked) {
        return;
    }

    extend_screen_display_time();

    switch_page(PAGE_SWITCH_PREVIOUS);
    debug_print("previous button click\n");
}

void apply_button_callback(uint32_t event) {
    static PageData current_page;
    current_page = *get_current_page();

    static ButtonDebounceCtx button_debounce_ctx = {.is_pressed = false,
                                                    .last_call = 0};

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME,
                              &button_debounce_ctx)) {
        return;
    }

    if (g_display_off) {
        toggle_screen(true);
        return;
    }

    extend_screen_display_time();

    if (g_warden_connected) {
        if (current_page.id == 0) {
            switch (g_alarm_state) {
            case (ALARM_STATE_NONE):
                toggle_alarm_state(ALARM_STATE_STANDBY_INIT);
                break;

            case (ALARM_STATE_STANDBY):
            case (ALARM_STATE_TRIGGERED):
                toggle_alarm_state(ALARM_STATE_NONE);
                break;

            default:
                break;
            }
        }
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

    if (g_navigation_blocked) {
        return;
    }

    extend_screen_display_time();

    switch_page(PAGE_SWITCH_NEXT);
    debug_print("next button click\n");
}

void alarm_buzzer_irq_callback() {
    static uint val = 0;
    static bool going_up = true;

    // keep it quiet for testing
    if (DEBUG) {
        post_pwm_irq(PD_BUZZER_PIN, 1000);
        return;
    }

    if (going_up) {
        val += 1;
    } else {
        val -= 5;
    }

    post_pwm_irq(PD_BUZZER_PIN, val * val);

    if (val <= 0) {
        going_up = true;
    }

    if (val >= 250) {
        going_up = false;
    }
}
