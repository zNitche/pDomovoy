#pragma once

#include "pico/stdlib.h"
#include <stdbool.h>

bool should_execute_repeating_function(uint32_t* next_runtime,
                                       uint32_t interval_ms);
