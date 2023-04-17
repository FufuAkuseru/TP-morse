#ifndef RCC_UTILS_H
#define RCC_UTILS_H

#include "stm32f10x_register_config.h"

/// @brief Initializes the RCC on the board so that the peripherals run at 36Mhz
void init_RCC();

#endif