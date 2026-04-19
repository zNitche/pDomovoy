#pragma once

#include "pico/stdlib.h"

typedef void (*button_callback)(uint gpio, uint32_t events);
typedef void (*buzzer_callback)();

typedef struct {
    uint32_t last_call;
    bool is_pressed;
} ButtonDebounceCtx;

enum DeviceStatus {
    PD_ADXL345_OK = 101,
    PD_ADXL345_ERROR = 102,
    PD_STANDBY_PREP = 203,
    PD_STANDBY_READY = 204,
    PD_ACCELERATION_TRIGGER = 205,
};

typedef struct {
    enum DeviceStatus status;
} McEventItem;

enum AlarmState {
    ALARM_STATE_NONE,
    ALARM_STATE_TRIGGERED,
    ALARM_STATE_STANDBY,
    ALARM_STATE_STANDBY_INIT,
};

typedef struct {
    float x;
    float y;
    float z;
} AccelerometerReading;
