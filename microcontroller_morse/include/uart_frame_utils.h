#ifndef UART_FRAME_UTILS_H
#define UART_FRAME_UTILS_H

#include "stm32f10x_register_config.h"

#define MAX_MSG_SIZE   250
#define MAX_QUEUE_SIZE 16

typedef struct uart_frame_s {
    uint8_t loop;
    uint8_t nb_iter;
    uint8_t short_timer;
    uint8_t medium_timer;
    uint8_t long_timer;
    uint8_t msg_length;
    uint8_t msg[MAX_MSG_SIZE];
    uint8_t frame_end;
} uart_frame_t;


typedef struct uart_frame_queue_s {
    uart_frame_t array[MAX_QUEUE_SIZE];
    uint8_t      index_front;
    uint8_t      index_rear;
    uint8_t      size;
} uart_frame_queue_t;

void init_queue(uart_frame_queue_t *q);

uint8_t queue_size(uart_frame_queue_t *q);

void queue_push(uart_frame_queue_t *q, uart_frame_t frame);

void queue_pop(uart_frame_queue_t *q, uart_frame_t *frame);

void emptying_queue(uart_frame_queue_t *q);

#endif