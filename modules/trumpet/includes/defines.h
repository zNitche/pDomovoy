#pragma once

#ifndef DEBUG
#define DEBUG 0
#endif

// pins
#define PD_PREVIOUS_BUTTON_PIN 15
#define PD_APPLY_BUTTON_PIN 17
#define PD_NEXT_BUTTON_PIN 14

// i2c

#define PD_AHT20_I2C i2c1
#define PD_AHT20_SDA_PIN 18
#define PD_AHT20_SCL_PIN 19

#define PD_SSD1306_I2C i2c0
#define PD_SSD1306_SDA_PIN 0
#define PD_SSD1306_SCL_PIN 1

// button actions
#define CLICK_DEBOUNCE_TIME 300

#define MIN_TIME_BETWEEN_CLICKS_FOR_ACTION 1200
