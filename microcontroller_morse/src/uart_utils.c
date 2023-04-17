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