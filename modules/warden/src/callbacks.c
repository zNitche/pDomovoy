#include "../includes/callbacks.h"
#include "../includes/delegates.h"

#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pico/time.h"

void gpio_irq_callback(uint gpio, uint32_t event) {
    switch (gpio) {
    case PD_ACTION_BUTTON_PIN:
        action_button_callback(event);
        break;

    default:
        break;
    }
}

void _action_for_continous_clicks(int* clicks) {
    switch (*clicks) {
    case CONTINOUS_CLICKS_TO_DISABLE_ALARM:
        *clicks = 0;
        disarm_alarm();

        break;

    default:
        break;
    }
}

void action_button_callback(uint32_t event) {
    static ButtonDebounceCtx button_debounce_ctx = {.is_pressed = false,
                                                    .last_call = 0};
    static int clicks_in_row = 0;

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME,
                              &button_debounce_ctx)) {
        return;
    }

    debug_print("action button click\n");

    if (g_btn_blocked) {
        return;
    }

    count_clicks_in_row(&button_debounce_ctx.last_call, &clicks_in_row,
                        MIN_TIME_BETWEEN_CLICKS_FOR_ACTION);

    if (g_alarm_state != ALARM_STATE_TRIGGERED) {
        clicks_in_row = 0;
        init_alarm_standby();

    } else {
        _action_for_continous_clicks(&clicks_in_row);
    }
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

bool blink_status_led_for_standby_callback(struct repeating_timer* t) {
    static bool state = false;

    gpio_put(PD_STATUS_LED_PIN, state);
    state = !state;

    return true;
}
