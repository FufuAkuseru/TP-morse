#ifndef UART_UTILS_H
#define UART_UTILS_H

#include "stm32f10x_register_config.h"
#include <stdbool.h>

typedef struct uart_message_s {
    uint8_t loop;
    uint8_t nb_ite;
    uint8_t timers[3];
    uint8_t msg_size;
    uint8_t msg[256];
} uart_message_t;

void init_USART2();
void uint8_array_to_uart_message(volatile uint8_t        *array,
                                 volatile uart_message_t *uart_msg);


#define MAX_QUEUE_SIZE 16
typedef struct uart_message_queue_s {
    uart_message_t array[MAX_QUEUE_SIZE];
    uint8_t        front;
    uint8_t        rear;
    uint8_t        size;
} uart_message_queue_t;

void    init_queue(uart_message_queue_t *queue);
uint8_t queue_size(uart_message_queue_t *queue);
void    queue_push(uart_message_queue_t *queue, uart_message_t uart_msg);
void    queue_pop(uart_message_queue_t *queue, uart_message_t *uart_msg);
void    empty_queue(uart_message_queue_t *queue);

#endif