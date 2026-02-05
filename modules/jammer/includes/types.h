#pragma once

#include "pico/stdlib.h"

typedef void (*button_callback)(uint gpio, uint32_t events);
typedef void (*buzzer_callback)();
