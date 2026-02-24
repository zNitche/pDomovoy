#pragma once

#include <stdbool.h>

#include "pico/stdlib.h"
#include "types.h"

void post_pwm_irq(uint gpio_pin, uint16_t pwm_level);
void set_pwm_irq_callback(uint gpio_pin, buzzer_callback irq_cb);
void set_pwm_irq_state(uint slice_num, bool state);
void enable_pwm_irq_on_pin(uint gpio_pin);
void disable_pwm_irq_on_pin(uint gpio_pin);
