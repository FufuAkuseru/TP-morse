#ifndef EXTI_UTILS_H
#define EXTI_UTILS_H

#include "stm32f10x_register_config.h"

typedef enum edge_mode_e { RISING_EDGE, FALLING_EDGE } edge_mode_t;

/// @brief Initializes a pin as external interrupt
/// @param exti_pin
void init_exti(Pin_t *exti_pin, edge_mode_t mode);

#endif