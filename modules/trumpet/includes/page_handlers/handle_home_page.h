#pragma once

#include "pdomovoy_common/types.h"
#include "pico_ssd1306/ssd1306.h"
#include "pico/stdlib.h"

void __load_bitmaps();
void handle_home_page(SSD1306_Frame* frame);
