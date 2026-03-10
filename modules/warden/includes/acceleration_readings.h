#pragma once

#include "../includes/types.h"
#include "pico_adxl345/adxl345.h"

accelerometer_reading get_accel_readings_mean(accelerometer_reading input[],
                                               size_t readings_count);

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c,
                                 accelerometer_reading total_accel[],
                                 size_t readings_count, int delay);
