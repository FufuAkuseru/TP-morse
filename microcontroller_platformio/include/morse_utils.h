#ifndef MORSE_UTILS_H
#define MORSE_UTILS_H

#include "stm32f10x_register_config.h"

char *convert_char(uint8_t c);
void  morse_to_blink(char *morse);

#endif