#pragma once
#include <stdbool.h>

#include "pico_adxl345/adxl345.h"

void get_accel_readings_mean(float input[][3], int input_length,
                             float output[3]);
void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c, int readings_count,
                                 float total_accel[][3], int delay);
void get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]);
bool check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                             float initial_accel_mean[3]);

void core_1();