#pragma once
#include <stdbool.h>

#include "pdomovoy_common/types.h"
#include "pico_adxl345/adxl345.h"

void _get_initial_accel_mean(ADXL345I2C* adxl345_i2c,
                             AccelerometerReading* output);
bool _check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                              AccelerometerReading* initial_accel_mean);
void _send_event_to_core_0(enum DeviceStatus status);

bool _check_for_trigger_for_axis(float initial_mean, float mean,
                                 float trigger_factor);

void core_1();
