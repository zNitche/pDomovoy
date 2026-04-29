#include "../includes/queue.h"

#include "pdomovoy_common/debug_print.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdbool.h>
#include <stdlib.h>

void pd_start_queue_timer(int32_t time_between, repeating_timer_callback_t cb,
                          repeating_timer_t* timer) {
    add_repeating_timer_ms(time_between, cb, NULL, timer);
};

void pd_stop_queue_timer(repeating_timer_t* timer) {
    cancel_repeating_timer(timer);
};

void pd_init_queue(FunctionsQueue* queue, int max_items) {
    queue->max_items = max_items;

    queue->items = malloc(sizeof(QueueFunction) * max_items);

    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
};

void pd_deinit_queue(FunctionsQueue* queue) { free(queue->items); }

bool pd_is_queue_full(FunctionsQueue* queue) {
    return queue->count == queue->max_items;
};

bool pd_is_queue_empty(FunctionsQueue* queue) { return queue->count == 0; };

bool pd_enqueue(FunctionsQueue* queue, QueueFunction function) {
    if (pd_is_queue_full(queue)) {
        return false;
    }

    queue->items[queue->tail] = function;
    queue->count += 1;

    queue->tail = (queue->tail + 1) % queue->max_items;

    debug_print("[PD_QUEUE] added to queue\n");

    return true;
};

void pd_move_queue(FunctionsQueue* queue) {
    debug_print("[PD_QUEUE] moving queue\n");

    if (pd_is_queue_empty(queue)) {
        return;
    }

    QueueFunction func = queue->items[queue->head];

    queue->head = (queue->head + 1) % queue->max_items;
    queue->max_items -= 1;

    if (queue->head == queue->tail) {
        queue->head = 0;
        queue->tail = 0;
    }

    func();

    debug_print("[PD_QUEUE] moved queue\n");
};
