#include "../includes/callbacks.h"

#include "../includes/button.h"
#include "../includes/core_1.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/pwm.h"
#include "pico/multicore.h"

void action_button_callback(uint gpio, uint32_t event) {
    if (!debounce_push_button(event, 300)) {
        return;
    }

    if (!alarm_triggered) {
        if (!alarm_in_standby) {
            multicore_launch_core1(core_1);
            alarm_in_standby = true;
        } else {
            multicore_reset_core1();
            alarm_in_standby = false;
        }
    }
}

void alarm_buzzer_irq_callback() { post_pwm_irq(PDA_BUZZER_PIN, 2000); }
