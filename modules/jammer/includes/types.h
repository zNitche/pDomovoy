#pragma once

#include "pico/stdlib.h"

typedef void (*button_callback)(uint gpio, uint32_t events);
typedef void (*buzzer_callback)();

enum DeviceStatus {
    PDA_ADXL345_OK = 101,
    PDA_ADXL345_ERROR = 102,
    PDA_ALARM_ON = 103,
    PDA_ALARM_OFF = 104,
    PDA_ALARM_TRIGGERED = 105,
};

typedef struct {
    enum DeviceStatus status;
} mc_event_item;
