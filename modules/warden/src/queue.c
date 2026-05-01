#include "../includes/queue.h"

#include "pdomovoy_common/debug_print.h"
#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void pd_init_queue(FunctionsQueue* queue, int max_items) {
    queue->max_items = max_items;
    queue->count = 0;

    queue->items = malloc(sizeof(QueueFunction) * max_items);

    queue->head = 0;
    queue->tail = -1;
};

void pd_deinit_queue(FunctionsQueue* queue) { free(queue->items); };

void pd_clear_queue(FunctionsQueue* queue) {
    queue->head = 0;
    queue->tail = -1;
    queue->count = 0;

    memset(queue->items, 0, sizeof(queue->items));

    debug_print("[PD_QUEUE] cleared\n");
};

bool pd_is_queue_full(FunctionsQueue* queue) {
    return queue->count == queue->max_items;
};

bool pd_is_queue_empty(FunctionsQueue* queue) { return queue->count == 0; };

bool pd_enqueue(FunctionsQueue* queue, QueueFunction function) {
    if (pd_is_queue_full(queue)) {
        return false;
    }

    queue->tail = (queue->tail + 1) % queue->max_items;

    queue->items[queue->tail] = function;
    queue->count++;

    debug_print("[PD_QUEUE] added to queue\n");

    return true;
};

void pd_move_queue(FunctionsQueue* queue) {
    if (pd_is_queue_empty(queue)) {
        return;
    }

    debug_print("[PD_QUEUE] moving queue | head:%d, tail:%d, count:%d\n",
                queue->head, queue->tail, queue->count);

    QueueFunction func = queue->items[queue->head];

    queue->head = (queue->head + 1) % queue->max_items;
    queue->count--;

    const int return_value = func();

    debug_print("[PD_QUEUE] moved queue with status: %d\n", return_value);
};
