#pragma once

#ifndef DEBUG
#define DEBUG 0
#endif

#define PD_WAIT_FOR_ALARM_ARM 15

// pins
#define PD_ADXL345_SDA_PIN 0
#define PD_ADXL345_SCL_PIN 1
#define PD_STATUS_LED_PIN 15
#define PD_BUZZER_PIN 16
#define PD_ACTION_BUTTON_PIN 18
#define PD_VOLTMETER_ADC_PIN 28
#define PD_VOLTMETER_ADC_ID 2

// button actions
#define CLICK_DEBOUNCE_TIME 1000

#define MIN_TIME_BETWEEN_CLICKS_FOR_ACTION 1500
#define CONTINOUS_CLICKS_TO_DISABLE_ALARM 5

// battery stats
#define BATTER_VOLTAGE_READINGS 30
#define MIN_BATTERY_VOLTAGE 3.3

// alarm buzzer
// tweaked to my buzzers, might need additional tweaks for different ones
#define PD_ALARM_BUZZER_DELAY_MS 300
#define PD_ALARM_BUZZER_ACTIVE_TIME_MS 100
#define PD_ALARM_BUZZER_HIGH_VALUE 2500
#define PD_ALARM_BUZZER_HIGH_VALUE_DEBUG 1000
