#include "pdomovoy_common/math.h"

#include "pico/stdlib.h"

float linear_transition(float value, float min, float max) {
    if (value <= min) {
        return 0.0;
    }

    if (value >= max) {
        return 100.0;
    }

    return (value - min) * 100.0;
}