#pragma once

#include "pico/stdlib.h"

typedef void (*button_callback)(uint gpio, uint32_t events);
typedef void (*buzzer_callback)();

enum DeviceStatus {
    PD_ADXL345_OK = 101,
    PD_ADXL345_ERROR = 102,
    PD_STANDBY_PREP = 203,
    PD_STANDBY_READY = 204,
    PD_ACCELERATION_TRIGGER = 205,
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
