#ifndef UART_FRAME_UTILS_H
#define UART_FRAME_UTILS_H

#include "stm32f10x_register_config.h"

#define MAX_MSG_SIZE   250
#define MAX_QUEUE_SIZE 16

/// @brief Data structure for a UART Frame
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

/// @brief Decodes an array of uint8_t into a UART Frame
/// @param usart2_rx_buffer The array to decode
/// @param frame            The frame to write into
void decode_frame(uint8_t *usart2_rx_buffer, uart_frame_t *frame);

/// @brief Data structure for a queue of UART Frame
typedef struct uart_frame_queue_s {
    uart_frame_t array[MAX_QUEUE_SIZE];
    uint8_t      index_front;
    uint8_t      index_rear;
    uint8_t      size;
} uart_frame_queue_t;


/// @brief Initializes a queue of UART Frames
/// @param uart_frame_queue The queue to initialize
void init_queue(uart_frame_queue_t *uart_frame_queue);

/// @brief Getter for the queue size
/// @param uart_frame_queue The queue to initialize
/// @return The size of the queue
uint8_t queue_size(uart_frame_queue_t *uart_frame_queue);

/// @brief Adds an UART Frame to the queue
/// @param uart_frame_queue The queue to add into
/// @param frame            The frame to add
void queue_push(uart_frame_queue_t *uart_frame_queue, uart_frame_t frame);

/// @brief Removes the UART Frame from the queue
/// @param uart_frame_queue The queue to remove from
/// @param frame            The frame to put the value in
void queue_pop(uart_frame_queue_t *uart_frame_queue, uart_frame_t *frame);

/// @brief Empties the queue
/// @param uart_frame_queue The queue to empty
void emptying_queue(uart_frame_queue_t *uart_frame_queue);

#endif