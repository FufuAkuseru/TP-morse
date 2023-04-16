#ifndef ADC_UTILS_H
#define ADC_UTILS_H

#include "stm32f10x_register_config.h"

/// @brief Initializes a given ADC for a certain pin
/// @param adc_choice Which ADC to use to convert
/// @param adc_pin    Pin read by the ADC
void init_ADC(ADC_TypeDef *adc_choice, Pin_t *adc_pin);

/// @brief Reads the value of a give ADC
/// @param adc Which ADC to read from
/// @return Read value
uint16_t reading_ADC(ADC_TypeDef *adc);

/// @brief Reads the value of a give ADC
/// @param adc Which ADC to read from
/// @param min The minimum value to map to
/// @param max The maximum value to map to
/// @return Read value
uint16_t reading_ADC_as(ADC_TypeDef *adc, uint16_t min, uint16_t max);

#endif