#include "../includes/callbacks.h"
#include "../includes/handlers.h"

#include "../includes/button.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/pwm.h"

void action_for_continous_clicks(int* clicks) {
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

    const uint32_t current_click_time = to_ms_since_boot(get_absolute_time());

    if (current_click_time - last_click_time <=
        MIN_TIME_BETWEEN_CLICKS_FOR_ACTION) {
        clicks_in_row += 1;
    } else {
        clicks_in_row = 0;
    }

    last_click_time = current_click_time;

    if (!alarm_triggered) {
        toggle_alarm_standby();
    } else {
        action_for_continous_clicks(&clicks_in_row);
    }
}

void alarm_buzzer_irq_callback() { post_pwm_irq(PDA_BUZZER_PIN, 2000); }
