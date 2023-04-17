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


#endif