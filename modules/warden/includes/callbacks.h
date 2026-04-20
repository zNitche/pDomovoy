#pragma once

#include "pdomovoy_common/types.h"
#include <stdbool.h>

void _action_for_continous_clicks(int* clicks);

void gpio_irq_callback(uint gpio, uint32_t event);
void action_button_callback(uint32_t event);
void alarm_buzzer_irq_callback();

bool blink_status_led_for_standby_callback(struct repeating_timer* t);
