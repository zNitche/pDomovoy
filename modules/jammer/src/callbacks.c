#include "../includes/callbacks.h"

#include "../includes/button.h"
#include "../includes/buzzer.h"
#include "../includes/defines.h"
#include "../includes/globals.h"

void action_button_callback(uint gpio, uint32_t event) {
  if (!debounce_push_button(event)) {
    return;
  }

  if (alarm_on) {
    disable_buzzer_pwm(PDA_BUZZER_PIN);

  } else {
    enable_buzzer_pwm(PDA_BUZZER_PIN);
  }

  alarm_on = !alarm_on;
}

void alarm_buzzer_irq_callback() { buzzer_pwm_call_irq(PDA_BUZZER_PIN); }
