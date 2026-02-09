#pragma once

#include "types.h"

void _action_for_continous_clicks(int* clicks);

void action_button_callback(uint gpio, uint32_t event);
void alarm_buzzer_irq_callback();
