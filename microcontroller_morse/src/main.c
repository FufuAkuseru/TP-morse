#include "exti_utils.h"
#include "gpio_utils.h"
#include "morse_utils.h"
#include "rcc_utils.h"
#include "stm32f10x_register_config.h"
#include "tim_utils.h"
#include "uart_frame_utils.h"
#include "uart_utils.h"

#include <stdbool.h>
#include <string.h>

Pin_t   led1           = {GPIOA, 5};
Pin_t   button1        = {GPIOC, 13};
uint8_t button_toggled = 0;

bool stop             = false;
bool tim2_done        = false;
bool tim3_done        = false;
bool tim4_done        = false;
bool message_received = false;

Pin_t tx = {GPIOA, 2};
Pin_t rx = {GPIOA, 3};

#define BUFFER_SIZE 256

volatile uint8_t  usart2_rx_buffer[BUFFER_SIZE];
volatile uint16_t usart2_rx_index = 0;


/// @brief Interrupt handler for button1 (built-in button)
void EXTI15_10_IRQHandler() {
    if (EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR       |= EXTI_PR_PR13;
        button_toggled = !button_toggled;
    }
}

/// @brief Interrupt handler for USART2
void USART2_IRQHandler() {
    /* check if USART2 received data */
    if (USART2->SR & USART_SR_RXNE) {
        /* read the received data */
        uint8_t received_data = USART2->DR;

        if (usart2_rx_index < BUFFER_SIZE) {
            usart2_rx_buffer[usart2_rx_index++] = received_data;
        }

        if (usart2_rx_index == 2) {
            if (!usart2_rx_buffer[0] && !usart2_rx_buffer[1]) {
                /* if the firs two bytes are 0, then issue a stop */
                stop             = true;
                tim2_done        = true;
                tim3_done        = true;
                tim4_done        = true;
                usart2_rx_index  = 0;
                message_received = true;
            }
        }

        uint8_t frame_length = usart2_rx_buffer[5] + 6;
        if (usart2_rx_index == frame_length) {
            /* full message has been received */
            usart2_rx_index  = 0;
            message_received = true;
        }
    }
}

/// @brief Interrupt handler for TIM2 (aka short)
void TIM2_IRQHandler() {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR  &= ~TIM_SR_UIF;
        tim2_done = true;
    }
}

/// @brief Interrupt handler for TIM3 (aka medium)
void TIM3_IRQHandler() {
    if (TIM3->SR & TIM_SR_UIF) {
        TIM3->SR  &= ~TIM_SR_UIF;
        tim3_done = true;
    }
}

/// @brief Interrupt handler for TIM4 (aka long)
void TIM4_IRQHandler() {
    if (TIM4->SR & TIM_SR_UIF) {
        TIM4->SR  &= ~TIM_SR_UIF;
        tim4_done = true;
    }
}

/// @brief Processing function for a frame
/// @param frame The data frame to process
void process_frame(uart_frame_t *frame) {
    set_tim(TIM2, frame->short_timer * 10);
    set_tim(TIM3, frame->medium_timer * 10);
    set_tim(TIM4, frame->long_timer * 10);

    if (frame->loop) {
        /* should the message repeat */
        while (!stop) {
            /* was a stop issued */
            string_to_morse(frame->msg, frame->msg_length);
        }
    } else {
        for (uint8_t i = 0; i < frame->nb_iter; ++i) {
            if (!stop) {
                /* was a stop issued */
                string_to_morse(frame->msg, frame->msg_length);
            }
        }
    }
}


int main(void) {
    init_RCC();

    /* init output */
    init_GPIO_Pin(&led1, WRITE_2MHz);
    init_GPIO_Pin(&tx, AF_WRITE_2MHz);
    init_GPIO_Pin(&rx, AF_READ);
    /* turns off output */
    clear_pin(&led1);

    init_USART2();
    init_tim(TIM2);
    init_tim(TIM3);
    init_tim(TIM4);

    /* init input as interrupt */
    init_exti(&button1, RISING_EDGE);

    uart_frame_queue_t q;
    init_queue(&q);

    while (1) {
        if (stop) {
            /* if stop was raised stop all timers */
            stop_tim(TIM2);
            stop_tim(TIM3);
            stop_tim(TIM4);
            stop             = false;
            message_received = false;
            memset(usart2_rx_buffer, ' ', BUFFER_SIZE);
            usart2_rx_index = 0;
            break;
        }
        if (message_received) {
            /* create frame from what was received in USART2 */
            uart_frame_t frame;
            memset(&frame, 0, sizeof(uart_frame_t));
            decode_frame(usart2_rx_buffer, &frame);
            queue_push(&q, frame);
            message_received = false;
            memset(usart2_rx_buffer, ' ', BUFFER_SIZE);
            usart2_rx_index = 0;
        }
        if (queue_size(&q)) {
            /* process each element of the queue */
            uart_frame_t frame;
            memset(&frame, 0, sizeof(uart_frame_t));
            queue_pop(&q, &frame);
            process_frame(&frame);
        }
        __asm("nop");
    }
    return 0;
}
