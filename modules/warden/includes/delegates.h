#pragma once

#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"

void set_alarm_state(enum AlarmState state);

void init_alarm_standby();
void enable_alarm_standby();
void disable_triggered_alarm();
