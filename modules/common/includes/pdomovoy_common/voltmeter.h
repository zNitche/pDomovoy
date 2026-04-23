#pragma once

#include "pico/stdlib.h"

void init_adc_voltmeter(int gpio);
float read_adc_voltage(float voltage_divider_ratio, int adc);
