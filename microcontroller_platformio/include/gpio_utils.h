#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

#include "stm32f10x_register_config.h"

/// @brief Sets a given pin to HIGH state
/// @param pin Pin to set
void set_pin(Pin_t *pin);

/// @brief Sets a given pin to LOW state
/// @param pin Pin to clear
void clear_pin(Pin_t *pin);

/// @brief Inverts a given pin
/// @param pin Pin to toggle
void toggle_pin(Pin_t *pin);

#endif