#pragma once

#include "pico/stdlib.h"
#include <stdbool.h>

typedef void (*QueueFunction)();

typedef struct {
    QueueFunction* items;
    int max_items;
    int head;
    int tail;
    int count;
} FunctionsQueue;

void pd_start_queue_timer(int32_t time_between, repeating_timer_callback_t cb,
                          repeating_timer_t* timer);
void pd_stop_queue_timer(repeating_timer_t* timer);

void pd_init_queue(FunctionsQueue* queue, int max_items);
void pd_deinit_queue(FunctionsQueue* queue);

bool pd_is_queue_full(FunctionsQueue* queue);
bool pd_is_queue_empty(FunctionsQueue* queue);

bool pd_enqueue(FunctionsQueue* queue, QueueFunction function);
void pd_move_queue(FunctionsQueue* queue);