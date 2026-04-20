#pragma once

#include "pico/stdlib.h"

void gpio_irq_callback(uint gpio, uint32_t event);

void previous_button_callback(uint32_t event);
void apply_button_callback(uint32_t event);
void next_button_callback(uint32_t event);
