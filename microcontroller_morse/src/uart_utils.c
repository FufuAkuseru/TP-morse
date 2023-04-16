#include "uart_utils.h"
#include <stdbool.h>
#include <string.h>

void init_USART2() {
    /* Enable USART2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure the USART2 baud rate to 9600 */
    USART2->BRR = 234 << USART_BRR_DIV_Mantissa_Pos;
    USART2->BRR |= 6 << USART_BRR_DIV_Fraction_Pos;

    /* Configure the USART2 settings */
    /* enabled update generation and interrupt on reception */
    USART2->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE;
    /* 8 data bits, parity control disabled, parity even */
    USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS);
    /* 1 stop bit */
    USART2->CR2 &= ~(USART_CR2_STOP_Msk);

    USART2->CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE);
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

    /* Enable USART2 interrupt */
    NVIC_EnableIRQ(USART2_IRQn);
}

void uint8_array_to_uart_message(volatile uint8_t        *array,
                                 volatile uart_message_t *uart_msg) {
    uart_msg->loop      = array[0];
    uart_msg->nb_ite    = array[1];
    uart_msg->timers[0] = array[2];
    uart_msg->timers[1] = array[3];
    uart_msg->timers[2] = array[4];
    uart_msg->msg_size  = array[5];
    for (uint8_t i = 0; i < uart_msg->msg_size; ++i) {
        uart_msg->msg[i] = array[6 + i];
    }
}

void init_queue(uart_message_queue_t *queue) {
    queue->front = 0;
    queue->rear  = 0;
    queue->size  = 0;
}


uint8_t queue_size(uart_message_queue_t *queue) { return queue->size; }

void queue_push(uart_message_queue_t *queue, uart_message_t uart_msg) {
    if (queue_size(queue) == MAX_QUEUE_SIZE) {
        return;
    }
    queue->array[queue->rear] = uart_msg;
    queue->rear               = (queue->rear + 1) % MAX_QUEUE_SIZE;
    ++(queue->size);
}

void queue_pop(uart_message_queue_t *queue, uart_message_t *uart_msg) {
    if (!queue_size(queue)) {
        return;
    }
    // memcpy(uart_msg, &queue->array[queue->front],
    //        sizeof(uint8_t) * (6 + queue->array[queue->front].msg_size));
    *uart_msg    = queue->array[queue->front];
    queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    --(queue->size);
}

void empty_queue(uart_message_queue_t *queue) {
    init_queue(queue);
    memset(queue, 0, MAX_QUEUE_SIZE);
}