#include "stm32f10x_register_config.h"
#include <errno.h>
#include <stddef.h>

/*Utility Section*/
void operation_on_address(volatile uint32_t *address, ope_t ope_type, int value,
                          unsigned char offset) {
    if (ope_type == CLEAR) {
        *address &= ~(value << offset);
    } else if (ope_type == SET) {
        *address |= value << offset;
    } else if (ope_type == TOGGLE) {
        *address ^= value << offset;
    }
}
/*Utility Section*/

/* CLOCK Section */
uint8_t get_APB2EN_register(GPIO_TypeDef *gpio_register) {
    if (gpio_register == NULL) {
        errno = EINVAL;
        return -1;
    }
    int offset = 0;
    if (gpio_register == GPIOA) {
        offset = 2;
    } else if (gpio_register == GPIOB) {
        offset = 3;
    } else if (gpio_register == GPIOC) {
        offset = 4;
    } else if (gpio_register == GPIOD) {
        offset = 5;
    } else if (gpio_register == GPIOE) {
        offset = 6;
    }
    return offset;
}

void enable_clock(Pin_t *pin) {
    int offset = get_APB2EN_register(pin->gpio_register);
    operation_on_address(&RCC->APB2ENR, CLEAR, 1, offset);
    operation_on_address(&RCC->APB2ENR, SET, 1, offset);
}
/* CLOCK Section */

/* GPIO->CR Section */
void operation_CRx(Pin_t *pin, ope_t ope_type, int value) {
    volatile uint32_t *addr   = (pin->pin_register < 8)
                                  ? &pin->gpio_register->CRL
                                  : &pin->gpio_register->CRH;
    uint8_t            offset = 4 * (pin->pin_register % 8);
    operation_on_address(addr, ope_type, value, offset);
}
/* GPIO->CR Section */

/* GPIO->CR Section */
void operation_ODR(Pin_t *pin, ope_t ope_type) {
    operation_on_address(&pin->gpio_register->ODR, ope_type, 1,
                         pin->pin_register);
}
/* GPIO->CR Section */

/* GPIO->BSRR Section */
void operation_BSRR(Pin_t *pin, ope_t ope_type) {
    int offset = pin->pin_register;
    if (ope_type == CLEAR) {
        offset += 16;
    }
    operation_on_address(&pin->gpio_register->BSRR, SET, 1, offset);
}
/* GPIO->BSRR Section */

/* GPIO->IDR Section */
uint32_t reading_IDR(Pin_t *pin) {
    return pin->gpio_register->IDR & (1 << pin->pin_register);
}
/* GPIO->IDR Section */
