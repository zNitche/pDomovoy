#pragma once

#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"

void set_alarm_state(enum AlarmState state);

void init_alarm_standby();
void arm_alarm();
void disarm_alarm();
