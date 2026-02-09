#pragma once
#include <stdbool.h>

#include "pico_adxl345/adxl345.h"
#include "types.h"

void get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]);
bool check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                             float initial_accel_mean[3]);
void send_event_to_core_0(enum DeviceStatus status);

void core_1();