#pragma once

#ifndef DEBUG
#define DEBUG 0
#endif

// display
#define PD_SCREEN_SAVER_AFTER 20000

// pins
#define PD_PREVIOUS_BUTTON_PIN 17
#define PD_APPLY_BUTTON_PIN 16
#define PD_NEXT_BUTTON_PIN 14

#define PD_VOLTMETER_ADC_PIN 28
#define PD_VOLTMETER_ADC_ID 2
#define PD_BUZZER_PIN 18

// i2c
#define PD_AHT20_I2C i2c0
#define PD_AHT20_SDA_PIN 0
#define PD_AHT20_SCL_PIN 1

#define PD_SSD1306_I2C i2c0
#define PD_SSD1306_SDA_PIN 0
#define PD_SSD1306_SCL_PIN 1

// button actions
#define CLICK_DEBOUNCE_TIME 1000

#define MIN_TIME_BETWEEN_CLICKS_FOR_ACTION 1500

// alarm buzzer
#define PD_ALARM_BUZZER_DELAY_MS 600
#define PD_ALARM_BUZZER_ACTIVE_TIME_MS 300

// intended to be placed next to bed, keep it quiet (I used 10uF capacitor
// instead of 1uF), therefore a higher value
#define PD_ALARM_BUZZER_HIGH_VALUE 8500
#define PD_ALARM_BUZZER_HIGH_VALUE_DEBUG 2000