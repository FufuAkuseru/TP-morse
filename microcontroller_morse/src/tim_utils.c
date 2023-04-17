#include "tim_utils.h"


/// @brief Enables a given Timer's clock
/// @param TIM Given Timer
void enable_TIM_APB(TIM_TypeDef *TIM) {
    if (TIM == TIM2) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    } else if (TIM == TIM3) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    } else if (TIM == TIM4) {
        RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }
}

/// @brief Enables a given Timer's interrupt
/// @param TIM Given Timer
void enable_TIM_IRQ(TIM_TypeDef *TIM) {
    if (TIM == TIM2) {
        NVIC_EnableIRQ(TIM2_IRQn);
    } else if (TIM == TIM3) {
        NVIC_EnableIRQ(TIM3_IRQn);
    } else if (TIM == TIM4) {
        NVIC_EnableIRQ(TIM4_IRQn);
    } else {
        return;
    }
}

void init_tim(TIM_TypeDef *TIM) {
    if (TIM == TIM1) {
        return;
    }
    enable_TIM_APB(TIM);
    TIM->PSC = 36000 - 1;
    TIM->EGR &= ~TIM_EGR_UG;
    TIM->EGR |= TIM_EGR_UG;

    TIM->DIER &= ~TIM_DIER_UIE;
    TIM->DIER |= TIM_DIER_UIE;
    enable_TIM_IRQ(TIM);
    TIM->CR1 &= ~TIM_CR1_CEN;
}

void set_tim(TIM_TypeDef *TIM, uint16_t ms) { TIM->ARR = ms - 1; }
void start_tim(TIM_TypeDef *TIM) { TIM->CR1 |= TIM_CR1_CEN; }
void stop_tim(TIM_TypeDef *TIM) { TIM->CR1 &= ~TIM_CR1_CEN; }