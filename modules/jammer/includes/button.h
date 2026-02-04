#pragma once

#include "pico/stdlib.h"
#include "types.h"

void init_button_irq(uint gpio, button_callback callback);
void toggle_button_callback(uint gpio, uint32_t event);