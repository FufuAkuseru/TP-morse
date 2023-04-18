#ifndef STM32F10xREGISTER_H
#define STM32F10xREGISTER_H

#include "stm32f103xb.h"

typedef struct s_pin {
    GPIO_TypeDef *gpio_register;
    unsigned      pin_register;
} Pin_t;

typedef enum e_ope { SET, CLEAR, TOGGLE } ope_t;

typedef enum e_iomode {
    READ          = 0b1000,
    WRITE_2MHz    = 0b0010,
    RESET         = 0b1111,
    ANALOG_READ   = 0b0000,
    AF_WRITE_2MHz = 0b1010,
    AF_READ       = 0b0100
} iomode_t;

#endif /* STM32F10xREGISTER_H */
