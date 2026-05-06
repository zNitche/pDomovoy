#pragma once

#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"

void __timestamp_to_hhmm(uint32_t timestamp, uint32_t* hours,
                         uint32_t* minutes);

void __get_boot_time_data(uint32_t* hours, uint32_t* minutes);
void __get_warden_connection_time_data(uint32_t* hours, uint32_t* minutes);
void __get_g_alarm_armed_time_data(uint32_t* hours, uint32_t* minutes);

void handle_time_stats_page(SSD1306_Frame* frame);
