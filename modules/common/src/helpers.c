#include "pdomovoy_common/helpers.h"

#include "pico/stdlib.h"
#include <stdbool.h>

bool should_execute_repeating_function(uint32_t* next_runtime,
                                       uint32_t interval_ms) {
    const uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time < *next_runtime) {
        return false;
    }

    *next_runtime = current_time + interval_ms;

    return true;
};