#pragma once

#include "../types.h"
#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"

void __timestamp_to_hhmm(uint32_t timestamp, SimpleTimeData* time_data);

void __get_boot_time_data(SimpleTimeData* time_data);
void __get_warden_connection_time_data(SimpleTimeData* time_data);
void __get_alarm_armed_time_data(SimpleTimeData* time_data);

void __render_row(SSD1306_Frame* frame, uint8_t y, char* label,
                  SimpleTimeData* time_data);

void handle_time_stats_page(SSD1306_Frame* frame);
