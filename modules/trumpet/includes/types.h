#pragma once

#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"

typedef void (*page_handler)(SSD1306_Frame* frame);

typedef struct {
    int id;
    char* title;
    page_handler handler;
} PageData;

typedef struct {
    uint32_t minutes;
    uint32_t hours;
} SimpleTimeData;

typedef struct {
    bool connected;
    uint32_t connected_timestamp;
    float battery_voltage;
    char software_version[6];
} WardenRemoteData;

enum PageSwitchDirection {
    PAGE_SWITCH_NEXT,
    PAGE_SWITCH_PREVIOUS,
};
