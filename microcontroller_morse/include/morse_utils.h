#ifndef STR_TO_BLINK_h
#define STR_TO_BLINK_h

#include "stm32f10x_register.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief Converts a uint8_t string into blinking the builtin LED in morse
/// @param str        The string to convert
/// @param str_length The length of the string
void string_to_morse(uint8_t *str, uint8_t str_length);

#endif