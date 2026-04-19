#pragma once

#include "pdomovoy_common/types.h"
#include "pico_adxl345/adxl345.h"

AccelerometerReading get_accel_readings_mean(AccelerometerReading input[],
                                             size_t readings_count);

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c,
                                 AccelerometerReading total_accel[],
                                 size_t readings_count, int delay);
