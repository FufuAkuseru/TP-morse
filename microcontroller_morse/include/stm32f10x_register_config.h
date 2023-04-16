#ifndef STM32F10xREGISTER_CONFIG_H
#define STM32F10xREGISTER_CONFIG_H

#include "stm32f10x_register.h"

/* Utility Section */
typedef enum e_ope { SET, CLEAR, TOGGLE } ope_t;
typedef enum e_iomode {
    READ          = 0b1000,
    WRITE_2MHz    = 0b0010,
    RESET         = 0b1111,
    ANALOG_READ   = 0b0000,
    AF_WRITE_2MHz = 0b1010,
    AF_READ       = 0b0100
} iomode_t;

/// @brief Does a CLEAR, SET, or TOGGLE on a given address with a given offset
/// @param address  Which address to do the operation on
/// @param ope_type Which operation to do
/// @param value    What value to put
/// @param offset   What bit offset to set the value to
void operation_on_address(volatile uint32_t *address, ope_t ope_type, int value,
                          unsigned char offset);
/* Utility Section */

/* CLOCK Section */
/// @brief Get the offset for a given GPIO register
/// @param gpio_register The GPIO register register to use
/// @return The offset in APB2EN to set the clock
uint8_t get_APB2EN_register(GPIO_TypeDef *gpio_register);

/// @brief Enables the clock for a given pin
/// @param pin The pin to enable the clock on
void enable_clock(Pin_t *pin);
/* CLOCK Section */

/* GPIO->CRx Section */
/// @brief Does a CLEAR, SET or TOGGLE on CRL/CRH for a pin
/// @param pin      Which pin to do the operation on
/// @param ope_type Which operation to do
/// @param value    What 4 bits value to set CRL/CRH
void operation_CRx(Pin_t *pin, ope_t ope_type, int value);

/// @brief Initialize a given GPIO pin (clock & mode)
/// @param pin    Which pin to initialize
/// @param iomode Which mode to use
void init_GPIO_Pin(Pin_t *pin, iomode_t iomode);
/* GPIO->CRx Section */

/* GPIO->ODR Section */
/// @brief CLEAR/SET/TOGGLE the ODR value for a pin
/// @param pin  Which pin to do the operation on
/// @param ope_type Which operation to do
void operation_ODR(Pin_t *pin, ope_t ope_type);
/* GPIO->ODR Section */

/* GPIO->BSRR Section */
/// @brief SET/RESET with BSRR for a pin
/// @param pin  Which pin to do the operation on
/// @param ope_type Which operation to do
void operation_BSRR(Pin_t *pin, ope_t ope_type);
/* GPIO->BSRR Section */

/* GPIO->IDR Section */
/// @brief Reads the IDR for a pin
/// @param pin Which pin to read for
/// @return Read value
uint32_t reading_IDR(Pin_t *pin);
/* GPIO->IDR Section */

#endif /* STM32F10xREGISTER_CONFIG_H */
