#pragma once

#include "pico_adxl345/adxl345.h"

void get_accel_readings_mean(float input[][3], int input_length,
                             float output[3]);

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c, int readings_count,
                                 float total_accel[][3], int delay);
