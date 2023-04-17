#include "rcc_utils.h"

void init_RCC() {
    /* Enable the internal high-speed clock (HSI) */
    RCC->CR |= RCC_CR_HSION;

    /* Wait until HSI is ready */
    while (!(RCC->CR & RCC_CR_HSIRDY)) {
        ;
    }

    RCC->CFGR &= ~RCC_CFGR_PLLMULL16;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;

    /* RCC->CFGR &= ~(RCC_CFGR_PPRE1);
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; */

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
        ;
    }

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        ;
    }
}