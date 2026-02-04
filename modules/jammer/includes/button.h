#pragma once

#include "pico/stdlib.h"
#include "types.h"

void init_button_irq(uint gpio, button_callback callback);
bool debounce_push_button(uint32_t event);