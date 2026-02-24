#include "../includes/callbacks.h"
#include "../includes/delegates.h"

#include "../includes/debug_print.h"
#include "../includes/button.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/pwm.h"

void _action_for_continous_clicks(int* clicks) {
    switch (*clicks) {
    case CONTINOUS_CLICKS_TO_DISABLE_ALARM:
        disable_triggered_alarm();
        *clicks = 0;

        break;
    default:
        break;
    }
}

void action_button_callback(uint gpio, uint32_t event) {
    static uint32_t last_click_time = 0;
    static int clicks_in_row = 0;

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME)) {
        return;
    }

    debug_print("action button click\n");

    if (g_btn_blocked) {
        return;
    }

    count_clicks_in_row(&last_click_time, &clicks_in_row,
                        MIN_TIME_BETWEEN_CLICKS_FOR_ACTION);

    if (g_alarm_state != ALARM_STATE_TRIGGERED) {
        init_alarm_standby();
    } else {
        _action_for_continous_clicks(&clicks_in_row);
    }
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

bool blink_status_led_for_standby_callback(struct repeating_timer* t) {
    static bool state = false;

    gpio_put(PD_STATUS_LED_PIN, state);
    state = !state;

    return true;
}
