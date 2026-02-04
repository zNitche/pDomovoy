#include "../includes/callbacks.h"
#include <stdio.h>

#include "../includes/button.h"
#include "../includes/buzzer.h"
#include "../includes/defines.h"
#include "../includes/globals.h"

void action_button_callback(uint gpio, uint32_t event) {
  if (!debounce_push_button(event)) {
    return;
  }

  printf("alarm on -> %d\n", alarm_on);
  alarm_on = !alarm_on;

  // FIXME
  // enable_buzzer_pwm(PDA_BUZZER_PIN);
}
