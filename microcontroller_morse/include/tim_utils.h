#ifndef TIM_UTILS_H
#define TIM_UTILS_H

#include <stdbool.h>

#include "stm32f10x_register.h"

/// @brief Initializes timer
/// @param TIM Timer to initialize (must be TIM2/3/4)
void init_tim(TIM_TypeDef *TIM);

/// @brief Sets a given timer to a set ms amount
/// @param TIM Timer to set
/// @param ms  Amount of ms at which point to the TIMER does an interrupt
void set_tim(TIM_TypeDef *TIM, uint16_t ms);

/// @brief Starts a given timer
/// @param TIM Timer to start
void start_tim(TIM_TypeDef *TIM);

/// @brief Stops a given timer
/// @param TIM Timer to stop
void stop_tim(TIM_TypeDef *TIM);

#endif