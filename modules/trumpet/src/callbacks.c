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
#include "pico/time.h"

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

    if (g_warden_remote_data.connected) {
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
    static bool alarm_on = false;
    static absolute_time_t next_alarm_time;

    const uint16_t HIGH_PWM_VALUE =
        DEBUG ? PD_ALARM_BUZZER_HIGH_VALUE_DEBUG : PD_ALARM_BUZZER_HIGH_VALUE;

    if (absolute_time_diff_us(get_absolute_time(), next_alarm_time) > 0) {
        post_pwm_irq(PD_BUZZER_PIN, alarm_on ? HIGH_PWM_VALUE : 0);
        return;
    }

    if (!alarm_on) {
        next_alarm_time = make_timeout_time_ms(PD_ALARM_BUZZER_ACTIVE_TIME_MS);
        alarm_on = true;
    } else {
        next_alarm_time = make_timeout_time_ms(PD_ALARM_BUZZER_DELAY_MS);
        alarm_on = false;
    }
}
