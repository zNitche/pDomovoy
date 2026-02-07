#pragma once

// pins
#define PDA_STATUS_LED_PIN 13
#define PDA_BUZZER_PIN 14
#define PDA_ACTION_BUTTON_PIN 12

// multicore fifo statuses
#define PDA_MCS_ADXL345_OK 1200
#define PDA_MCS_ADXL345_ERROR 1205
#define PDA_MCS_ALARM_ON 1201
#define PDA_MCS_ALARM_OFF 1202
#define PDA_MCS_ALARM_TRIGGERED 1203
