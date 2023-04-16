#ifndef MORSE_UTILS_H
#define MORSE_UTILS_H

#include "stm32f10x_register_config.h"
#include "uart_utils.h"

char *convert_char(uint8_t c);
void  morse_to_blink(char *morse);
void  uart_message_to_blink(uart_message_t *uart_msg);

#endif