#include "gpio_utils.h"

void set_pin(Pin_t *pin) { operation_ODR(pin, SET); }
void clear_pin(Pin_t *pin) { operation_ODR(pin, CLEAR); }
void toggle_pin(Pin_t *pin) { operation_ODR(pin, TOGGLE); }

void init_GPIO_Pin(Pin_t *pin, iomode_t iomode) {
    enable_clock(pin);
    operation_CRx(pin, CLEAR, RESET);
    operation_CRx(pin, SET, iomode);
}