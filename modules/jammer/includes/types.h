#pragma once

#include "pico/stdlib.h"

typedef void (*button_callback)(uint gpio, uint32_t events);
typedef void (*buzzer_callback)();

enum DeviceStatus {
    PDA_ADXL345_OK = 101,
    PDA_ADXL345_ERROR = 102,
    PDA_STANDBY_PREP = 203,
    PDA_STANDBY_READY = 204,
    PDA_ACCELERATION_TRIGGER = 205,
};

typedef struct {
    enum DeviceStatus status;
} mc_event_item;

enum AlarmState {
    ALARM_STATE_NONE,
    ALARM_STATE_TRIGGERED,
    ALARM_STATE_STANDBY,
    ALARM_STATE_STANDBY_INIT,
};
