#include "string.h"
#include <stdio.h>

#include "uart_frame_utils.h"

void decode_frame(uint8_t *usart2_rx_buffer, uart_frame_t *frame) {
    frame->loop         = usart2_rx_buffer[0];
    frame->nb_iter      = usart2_rx_buffer[1];
    frame->short_timer  = usart2_rx_buffer[2];
    frame->medium_timer = usart2_rx_buffer[3];
    frame->long_timer   = usart2_rx_buffer[4];
    frame->msg_length   = usart2_rx_buffer[5];
    for (int j = 0; j < frame->msg_length; j++) {
        frame->msg[j] = usart2_rx_buffer[j + 6];
    }
}