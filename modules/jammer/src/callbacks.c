#include "../includes/callbacks.h"
#include "../includes/handlers.h"

#include "../includes/button.h"
#include "../includes/defines.h"
#include "../includes/pwm.h"

void action_button_callback(uint gpio, uint32_t event) {
    if (!debounce_push_button(event, 300)) {
        return;
    }

    toggle_alarm_standby();
}

void alarm_buzzer_irq_callback() { post_pwm_irq(PDA_BUZZER_PIN, 2000); }
