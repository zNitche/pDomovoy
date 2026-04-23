#pragma once

#include "pdomovoy_common/types.h"
#include "pico_ssd1306/ssd1306.h"
#include "pico/stdlib.h"

void __load_bitmaps();
void __handle_env_sensors(SSD1306_Frame* frame, float aht20_readings[2]);
void __handle_batteries(SSD1306_Frame* frame);
void handle_home_page(SSD1306_Frame* frame);
