#ifndef STR_TO_BLINK_h
#define STR_TO_BLINK_h

#include "stm32f10x_register_config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_to_morse(uint8_t *str, int str_length);

#endif