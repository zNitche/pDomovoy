#pragma once

#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"

void __pd_setup_gap_advertisements();
void __pd_client_state_cleanup(bool reset_alarm_state);

void toggle_alarm_state(enum AlarmState state);
