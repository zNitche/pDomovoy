#pragma once

#include <stdbool.h>

#include "pico/stdlib.h"
#include "types.h"

void post_pwm_irq(uint gpio_pin, uint16_t pwm_level);
void init_buzzer_pwm(uint gpio_pin, buzzer_callback irq_cb);
void set_pwm_irq_state(uint slice_num, bool state);
void enable_buzzer_pwm(uint gpio_pin);
void disable_buzzer_pwm(uint gpio_pin);
