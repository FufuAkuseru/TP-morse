#include "exti_utils.h"
#include "gpio_utils.h"
#include "morse_utils.h"
#include "rcc_utils.h"
#include "tim_utils.h"
#include "uart_utils.h"

#include <stdbool.h>
#include <string.h>

Pin_t led1    = {GPIOA, 5};
Pin_t button1 = {GPIOC, 13};
Pin_t tx      = {GPIOA, 2};
Pin_t rx      = {GPIOA, 3};

bool button1_on       = false;
bool tim2_done        = false;
bool tim3_done        = false;
bool tim4_done        = false;
bool stop             = false;
bool message_received = false;

#define BUFFER_SIZE 256

volatile uint8_t  usart2_rx_buffer[BUFFER_SIZE];
volatile uint16_t usart2_rx_index = 0;


void EXTI15_10_IRQHandler() {
    if (EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR   |= EXTI_PR_PR13;
        button1_on = !button1_on;
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim2_done = true;
    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim3_done = true;
    }
}

void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim4_done = true;
    }
}

void USART2_IRQHandler(void) {
    // Check if USART2 received data
    if ((USART2->SR & USART_SR_RXNE) != 0) {
        // Read the received data
        uint8_t received_data = USART2->DR;

        if (usart2_rx_index < BUFFER_SIZE) {
            usart2_rx_buffer[usart2_rx_index++] = received_data;
        }
        if (usart2_rx_index == 2) {
            if (!usart2_rx_buffer[0] && !usart2_rx_buffer[1]) {
                stop             = true;
                tim2_done        = true;
                tim3_done        = true;
                tim4_done        = true;
                usart2_rx_index  = 0;
                message_received = true;
            }
        }
        // Check if the entire message has been received
        if (usart2_rx_index == (usart2_rx_buffer[5] + 6)) {
            // Reset the byte count
            usart2_rx_index  = 0;
            message_received = true;
        }
    }
}


int main(void) {
    init_RCC();
    /* init output */
    init_GPIO_Pin(&led1, WRITE_2MHz);
    init_GPIO_Pin(&tx, AF_WRITE_2MHz);
    init_GPIO_Pin(&rx, AF_READ);

    init_USART2();

    /* turns off output */
    clear_pin(&led1);

    /* init input as interrupt */
    init_exti(&button1, RISING_EDGE);

    uart_message_queue_t uart_queue;
    init_queue(&uart_queue);

    init_tim(TIM2);
    set_tim(TIM2, 100);
    init_tim(TIM3);
    set_tim(TIM3, 300);
    init_tim(TIM4);
    set_tim(TIM4, 700);
    while (1) {
        if (stop) {
            stop_tim(TIM2);
            stop_tim(TIM3);
            stop_tim(TIM4);
            empty_queue(&uart_queue);
        }
        if (queue_size(&uart_queue)) {
            uart_message_t popped_msg;
            queue_pop(&uart_queue, &popped_msg);
            uart_message_to_blink(&popped_msg);
        }
        if (message_received) {
            uart_message_t uart_msg;
            uint8_array_to_uart_message(usart2_rx_buffer, &uart_msg);
            queue_push(&uart_queue, uart_msg);
            usart2_rx_index = 0;
            memset(usart2_rx_buffer, 1, 256);
            message_received = true;
        }
        __asm("nop");
    }
    return 0;
}
