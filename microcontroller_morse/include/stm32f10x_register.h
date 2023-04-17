#ifndef STM32F10xREGISTER_H
#define STM32F10xREGISTER_H

#include "stm32f103xb.h"

typedef struct s_pin {
  GPIO_TypeDef *gpio_register;
  unsigned pin_register;
} Pin_t;

#endif /* STM32F10xREGISTER_H */
