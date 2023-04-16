#include "exti_utils.h"
#include "gpio_utils.h"
#include "morse_utils.h"
#include "rcc_utils.h"
#include "tim_utils.h"
#include "uart_utils.h"

#include <stdbool.h>

Pin_t led1    = {GPIOA, 5};
Pin_t button1 = {GPIOC, 13};
Pin_t tx      = {GPIOA, 2};
Pin_t rx      = {GPIOA, 3};

bool led_on           = false;
bool tim2_done        = false;
bool tim3_done        = false;
bool tim4_done        = false;
bool stop             = false;
bool message_received = false;

void EXTI15_10_IRQHandler() {
    if (EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR |= EXTI_PR_PR13;
        led_on   = !led_on;
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim2_done = !tim2_done;
    }
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim3_done = !tim3_done;
    }
}

void TIM4_IRQHandler(void) {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR &= ~TIM_SR_UIF;
        // Timer update event occurred
        tim4_done = !tim4_done;
    }
}

#define BUFFER_SIZE 256

volatile uint8_t  usart2_rx_buffer[BUFFER_SIZE];
volatile uint16_t usart2_rx_index = 0;

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

    uart_message_queue_t q;
    init_queue(&q);

    init_tim(TIM2);
    set_tim(TIM2, 100);
    init_tim(TIM3);
    set_tim(TIM3, 300);
    init_tim(TIM4);
    set_tim(TIM4, 700);
    while (1) {
        if (message_received) {
            uart_message_t uart_msg;
            uint8_array_to_uart_message(usart2_rx_buffer, &uart_msg);
            queue_push(&q, uart_msg);
            message_received = 0;
        }

        if (queue_size(&q)) {
            uart_message_t popped_msg;
            queue_pop(&q, &popped_msg);
            uart_message_to_blink(&popped_msg);
        }
        if (stop) {
            stop_tim(TIM2);
            stop_tim(TIM3);
            stop_tim(TIM4);
            while (queue_size(&q)) {
                uart_message_t popped_msg;
                queue_pop(&q, &popped_msg);
            }
        }
        __asm("nop");
    }
    return 0;
}
