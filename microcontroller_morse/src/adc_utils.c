#include "adc_utils.h"

/// @brief Selects the correct ADC channel for a given pin
/// @param pin The given pin
/// @return The ADC channel
uint8_t select_channel(Pin_t *pin) {
    if (pin->gpio_register == GPIOA) {
        if (pin->pin_register < 8) {
            return pin->pin_register;
        }
    } else if (pin->gpio_register == GPIOB) {
        if (pin->pin_register < 2) {
            return 8 + pin->pin_register;
        }
    } else if (pin->gpio_register == GPIOC) {
        if (pin->pin_register < 6) {
            return 10 + pin->pin_register;
        }
    }
    return -1;
}

void init_ADC(ADC_TypeDef *adc_choice, Pin_t *adc_pin) {
    if (adc_choice == ADC1) {
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    } else if (adc_choice == ADC2) {
        RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
    } else {
        return;
    }

    init_GPIO_Pin(adc_pin, ANALOG_READ);
    uint8_t channel = select_channel(adc_pin);
    if (channel == -1) {
        return;
    }

    /* set up sequence */
    adc_choice->SQR1 &= ~(0b1111 << ADC_SQR1_L_Pos);
    adc_choice->SQR1 |= 0b0000 << ADC_SQR1_L_Pos;
    adc_choice->SQR3 &= ~(0b11111 << ADC_SQR3_SQ1_Pos);
    adc_choice->SQR3 |= channel << ADC_SQR3_SQ1_Pos;

    /* sample 239.5 cycles */
    adc_choice->SMPR2 &= ~(0b111 << ADC_SMPR2_SMP0_Pos);
    adc_choice->SMPR2 |= 0b111 << ADC_SMPR2_SMP0_Pos;

    /* turn on ADC */
    adc_choice->CR2 &= ~(1 << ADC_CR2_ADON_Pos);
    adc_choice->CR2 |= 1 << ADC_CR2_ADON_Pos;

    /* ADC calibration */
    adc_choice->CR2 |= (1 << ADC_CR2_CAL_Pos);
    while (adc_choice->CR2 & ADC_CR2_CAL_Msk) {
        __asm("nop");
    }
}

uint16_t reading_ADC(ADC_TypeDef *adc) {
    operation_on_address(&adc->CR2, CLEAR, 1, ADC_CR2_SWSTART_Pos);
    operation_on_address(&adc->CR2, SET, 1, ADC_CR2_SWSTART_Pos);
    while (!(adc->SR & ADC_SR_EOC)) {
        __asm("nop");
    }
    return adc->DR & ADC_DR_DATA_Msk;
}

uint16_t reading_ADC_as(ADC_TypeDef *adc, uint16_t min, uint16_t max) {
    operation_on_address(&adc->CR2, CLEAR, 1, ADC_CR2_SWSTART_Pos);
    operation_on_address(&adc->CR2, SET, 1, ADC_CR2_SWSTART_Pos);
    while (!(adc->SR & ADC_SR_EOC)) {
        __asm("nop");
    }
    uint16_t value     = adc->DR & ADC_DR_DATA_Msk;
    uint16_t old_range = 4095 - 0; /* 2^12 because 12 bits ADC */
    uint16_t new_range = max - min;
    return (((uint32_t) (value - 0) * new_range) / old_range) + min;
}