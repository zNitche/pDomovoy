#pragma once

#include "pico_aht20/aht20.h"
#include "pico_aht20/defines.h"
#include "pico_ssd1306/ssd1306.h"

AHT20_I2C init_aht20();
SSD1306_I2C init_ssd1306();
