#pragma once

#include "types.h"

int _is_battery_voltage_low();
void _wait_for_alarm_standby();
void _process_event(mc_event_item* event);
void core_0();
