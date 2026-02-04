#pragma once

#include "pico/stdlib.h"

void buzzer_pwm_call_irq(uint gpio_pin);
void init_buzzer_pwm(uint gpio_pin);
void enable_buzzer_pwm(uint gpio_pin);
void disable_buzzer_pwm(uint gpio_pin);
