#include "exti_utils.h"
#include "gpio_utils.h"
#include "morse_utils.h"
#include "rcc_utils.h"
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

void EXTI15_10_IRQHandler() {
    if (EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR       |= EXTI_PR_PR13;
        button_toggled = !button_toggled;
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

    while (1) {
        if (message_received) {
            uart_frame_t frame;
            memset(&frame, 0, sizeof(uart_frame_t));

            frame.loop         = usart2_rx_buffer[0];
            frame.nb_iter      = usart2_rx_buffer[1];
            frame.short_timer  = usart2_rx_buffer[2];
            frame.medium_timer = usart2_rx_buffer[3];
            frame.long_timer   = usart2_rx_buffer[4];
            frame.msg_length   = usart2_rx_buffer[5];
            for (int j = 0; j < frame.msg_length; j++) {
                frame.msg[j] = usart2_rx_buffer[j + 6];
            }

            set_tim(TIM2, frame.short_timer * 10);
            set_tim(TIM3, frame.medium_timer * 10);
            set_tim(TIM4, frame.long_timer * 10);

            if (frame.loop) {
                while (!stop) {
                    string_to_morse(frame.msg, frame.msg_length);
                }
            } else {
                for (uint8_t i = 0; i < frame.nb_iter; ++i) {
                    if (!stop) {
                        string_to_morse(frame.msg, frame.msg_length);
                    }
                }
            }
            message_received = false;
            memset(usart2_rx_buffer, ' ', BUFFER_SIZE);
            usart2_rx_index = 0;
        }
        __asm("nop");
    }
    return 0;
}
