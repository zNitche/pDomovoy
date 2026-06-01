#pragma once

#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"

void __load_bitmaps();
void __handle_env_sensors(SSD1306_Frame* frame, float aht20_readings[2]);

void __render_battery_details_row(SSD1306_Frame* frame, uint8_t y, char* label,
                                  float voltage);
void __handle_batteries(SSD1306_Frame* frame);

void __handle_alarm_bell(SSD1306_Frame* frame);

void handle_home_page(SSD1306_Frame* frame);
