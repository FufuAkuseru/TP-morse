#include "gpio_utils.h"

/// @brief Get the offset for a given GPIO register
/// @param gpio_register The GPIO register register to use
/// @return The offset in APB2EN to set the clock
uint8_t get_APB2EN_register(GPIO_TypeDef *gpio_register) {
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

/// @brief Enables the clock for a given pin
/// @param pin The pin to enable the clock on
void enable_clock(Pin_t *pin) {
    int offset   = get_APB2EN_register(pin->gpio_register);
    RCC->APB2ENR &= ~(1 << offset);
    RCC->APB2ENR |= 1 << offset;
}

void init_GPIO_Pin(Pin_t *pin, iomode_t iomode) {
    enable_clock(pin);
    if (pin->pin_register < 8) {
        pin->gpio_register->CRL &= ~(RESET << (4 * (pin->pin_register % 8)));
        pin->gpio_register->CRL |= iomode << (4 * (pin->pin_register % 8));
    } else {
        pin->gpio_register->CRH &= ~(RESET << (4 * (pin->pin_register % 8)));
        pin->gpio_register->CRH |= iomode << (4 * (pin->pin_register % 8));
    }
}

void set_pin(Pin_t *pin) { pin->gpio_register->ODR |= 1 << pin->pin_register; }

void clear_pin(Pin_t *pin) {
    pin->gpio_register->ODR &= ~(1 << pin->pin_register);
}

void toggle_pin(Pin_t *pin) {
    pin->gpio_register->ODR ^= 1 << pin->pin_register;
}