#include "exti_utils.h"
#include "gpio_utils.h"

/// @brief Sets up the correct AFIO_EXTICR for a given pin
/// @param exti_pin A given pin
void set_AFIO_EXTICR(Pin_t *exti_pin) {
    uint8_t value = 0b0000;
    if (exti_pin->gpio_register == GPIOA) {
        value = 0b0000;
    } else if (exti_pin->gpio_register == GPIOB) {
        value = 0b0001;
    } else if (exti_pin->gpio_register == GPIOC) {
        value = 0b0010;
    } else if (exti_pin->gpio_register == GPIOD) {
        value = 0b0011;
    } else if (exti_pin->gpio_register == GPIOE) {
        value = 0b0100;
    } else {
        return;
    }

    switch (exti_pin->pin_register) {
        case 0:
        case 1:
        case 2:
        case 3:
            AFIO->EXTICR[0] = 0xF << ((exti_pin->pin_register % 4) * 4);
            AFIO->EXTICR[0] = value << ((exti_pin->pin_register % 4) * 4);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            AFIO->EXTICR[1] = 0xF << ((exti_pin->pin_register % 4) * 4);
            AFIO->EXTICR[1] = value << ((exti_pin->pin_register % 4) * 4);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            AFIO->EXTICR[2] = 0xF << ((exti_pin->pin_register % 4) * 4);
            AFIO->EXTICR[2] = value << ((exti_pin->pin_register % 4) * 4);
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            AFIO->EXTICR[3] = 0xF << ((exti_pin->pin_register % 4) * 4);
            AFIO->EXTICR[3] = value << ((exti_pin->pin_register % 4) * 4);
            break;
        default:
            return;
    }
}

/// @brief Enables a given EXTI's interrupt
/// @param exti_pin Given external interrupt pin
void enable_EXTI_IRQ(Pin_t *exti_pin) {
    IRQn_Type IRQn;
    switch (exti_pin->pin_register) {
        case 0:
            IRQn = EXTI0_IRQn;
            break;
        case 1:
            IRQn = EXTI1_IRQn;
            break;
        case 2:
            IRQn = EXTI2_IRQn;
            break;
        case 3:
            IRQn = EXTI3_IRQn;
            break;
        case 4:
            IRQn = EXTI4_IRQn;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            IRQn = EXTI9_5_IRQn;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            IRQn = EXTI15_10_IRQn;
            break;
        default:
            return;
    }
    NVIC_EnableIRQ(IRQn);
}

void init_exti(Pin_t *exti_pin, edge_mode_t mode) {
    init_GPIO_Pin(exti_pin, AF_READ);
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    set_AFIO_EXTICR(exti_pin);
    EXTI->IMR  &= ~(1 << exti_pin->pin_register);
    EXTI->IMR  |= 1 << exti_pin->pin_register;
    EXTI->FTSR &= ~(1 << exti_pin->pin_register);
    EXTI->RTSR &= ~(1 << exti_pin->pin_register);

    switch (mode) {
        case RISING_EDGE:
            EXTI->RTSR |= 1 << exti_pin->pin_register;
            break;
        default:
            EXTI->FTSR |= 1 << exti_pin->pin_register;
            break;
    }

    enable_EXTI_IRQ(exti_pin);
}
