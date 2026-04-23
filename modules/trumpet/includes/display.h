#pragma once

#include "pico/stdlib.h"
#include "pico/time.h"

void extend_screen_display_time();
void toggle_screen(bool state);
bool _screen_saver_cb(__unused repeating_timer_t* t);
void start_screen_save_timer();
void stop_screen_save_timer();
