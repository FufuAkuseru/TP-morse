#include "rcc_utils.h"

void init_RCC() {
    /* enables the internal high-speed clock (HSI) */
    RCC->CR |= RCC_CR_HSION;

    /* waits until HSI is ready */
    while (!(RCC->CR & RCC_CR_HSIRDY)) {
        ;
    }

    /* sets PLL to HSI * 9 */
    RCC->CFGR &= ~RCC_CFGR_PLLMULL16;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;

    /* turns on PLL */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
        ;
    }

    /* set peripherals clock as PLL */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        ;
    }
}