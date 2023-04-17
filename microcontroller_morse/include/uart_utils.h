#ifndef UART_UTILS_H
#define UART_UTILS_H

#include "stm32f10x_register_config.h"

/// @brief Initializes USART2 to use 96000 bauds, 8 data bits, no parity control
///        with an even parity and 1 stop bit
void init_USART2();

#endif