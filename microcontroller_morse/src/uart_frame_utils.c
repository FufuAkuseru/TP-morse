#include "string.h"
#include <stdio.h>

#include "uart_frame_utils.h"

void init_queue(uart_frame_queue_t *q) {
    q->index_front = 0;
    q->index_rear  = 0;
    q->size        = 0;
}

uint8_t queue_size(uart_frame_queue_t *q) { return q->size; }

void queue_push(uart_frame_queue_t *q, uart_frame_t frame) {
    if (queue_size(q) == MAX_QUEUE_SIZE) {
        return;
    }
    q->array[q->index_rear] = frame;
    q->index_rear           = (q->index_rear + 1) % MAX_QUEUE_SIZE;
    ++(q->size);
}

void queue_pop(uart_frame_queue_t *q, uart_frame_t *frame) {
    if (!queue_size(q)) {
        return;
    }
    *frame         = q->array[q->index_front];
    q->index_front = (q->index_front + 1) % MAX_QUEUE_SIZE;
    --(q->size);
}

void emptying_queue(uart_frame_queue_t *q) { init_queue(q); }