#include "../includes/callbacks.h"

#include "../includes/button.h"
#include "../includes/pwm.h"
#include "../includes/defines.h"
#include "../includes/globals.h"

void action_button_callback(uint gpio, uint32_t event) {
  if (!debounce_push_button(event)) {
    return;
  }

  if (alarm_on) {
    disable_pwm_irq_on_pin(PDA_BUZZER_PIN);

  } else {
    enable_pwm_irq_on_pin(PDA_BUZZER_PIN);
  }

  alarm_on = !alarm_on;
}

void alarm_buzzer_irq_callback() { post_pwm_irq(PDA_BUZZER_PIN, 2000); }
