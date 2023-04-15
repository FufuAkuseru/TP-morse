#include "exti_utils.h"
#include "gpio_utils.h"
#include "morse_utils.h"
#include "rcc_utils.h"
#include "tim_utils.h"
#include "uart_utils.h"

Pin_t led1    = {GPIOA, 5};
Pin_t button1 = {GPIOC, 13};
Pin_t tx      = {GPIOA, 2};
Pin_t rx      = {GPIOA, 3};

uint8_t led_on           = 0;
uint8_t tim2_done        = 0;
uint8_t tim3_done        = 0;
uint8_t tim4_done        = 0;
uint8_t message_received = 0;


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

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {
        uint8_t data = USART2->DR; // read the received data

        // Echo back the received data
        USART2->DR = data;
        while (!(USART2->SR & USART_SR_TXE)) {
            ; // wait until transmission is complete
        }

        message_received = 1; // set message_received flag to true
    }
}

int main(void) {
    init_RCC();
    /* init output */
    init_GPIO_Pin(&led1, WRITE_2MHz);
    init_GPIO_Pin(&tx, AF_WRITE_2MHz);
    init_GPIO_Pin(&rx, AF_READ);

    init_UART2();

    /* turns off output */
    clear_pin(&led1);

    /* init input as interrupt */
    init_exti(&button1, RISING_EDGE);

    init_tim(TIM2);
    set_tim(TIM2, 1000);
    init_tim(TIM3);
    set_tim(TIM3, 200);
    init_tim(TIM4);
    set_tim(TIM4, 300);

    while (1) {
        set_pin(&led1);
        start_tim(TIM2);
        while (!tim2_done) {
            ;
        }
        stop_tim(TIM2);
        tim2_done = false;
        clear_pin(&led1);
        __asm("nop");
    }
    return 0;
}
