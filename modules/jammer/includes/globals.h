#pragma once

#include "pico/util/queue.h"
#include <stdbool.h>

extern bool alarm_on;

extern queue_t core0_events_queue;
extern queue_t core1_events_queue;
