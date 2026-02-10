#pragma once

#include "pico/time.h"

void blink_blocking(int pin, int times, int time_between);

void blink_untill_start(int pin, int32_t time_between, repeating_timer_callback_t cb);
void blink_untill_stop(int pin);