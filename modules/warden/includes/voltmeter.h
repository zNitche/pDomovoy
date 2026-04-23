#pragma once

#include "pico/stdlib.h"
#include <stdbool.h>

bool _voltmeter_cb(repeating_timer_t* t);
void start_voltmeter_bg();
void stop_voltmeter_bg();
