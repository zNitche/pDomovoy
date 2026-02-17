#pragma once

#include "pico/time.h"
#include <stdbool.h>

void blink_blocking(int pin, int times, int time_between);

void blink_untill_start(int32_t time_between, repeating_timer_callback_t cb,
                        repeating_timer_t* timer, bool reclaim_on_init);
void blink_untill_stop(int pin, repeating_timer_t* timer);

void cyw34_blink_untill_start(int32_t time_between, repeating_timer_t* timer,
                              bool reclaim_on_init);
void cyw34_blink_untill_stop(repeating_timer_t* timer);

void blink_onboard_led_blocking(int times, int time_between);
bool _blink_onboard_led_cb(__unused repeating_timer_t* t);
