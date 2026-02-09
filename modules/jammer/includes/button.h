#pragma once

#include "pico/stdlib.h"
#include "types.h"

void init_button_irq(uint gpio, button_callback callback);
bool debounce_push_button(uint32_t event, int min_delay_between_clicks);
void count_clicks_in_row(uint32_t* last_click_time, int* clicks_in_row,
                         int min_time_between_clicks);