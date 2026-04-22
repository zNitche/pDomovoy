#pragma once

#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"


typedef void (*page_handler)(SSD1306_Frame* frame);

typedef struct {
    char* title;
    page_handler handler;
} PageData;

enum PageSwitchDirection {
    PAGE_SWITCH_NEXT,
    PAGE_SWITCH_PREVIOUS,
};
