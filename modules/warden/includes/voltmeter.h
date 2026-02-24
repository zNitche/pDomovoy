#pragma once

#include "pico/stdlib.h"
#include <stdbool.h>

void init_adc_voltmeter(int gpio, int adc);
float read_adc_voltage();

bool _voltmeter_cb(__unused repeating_timer_t* t);
void start_voltmeter_bg();
void stop_voltmeter_bg();
