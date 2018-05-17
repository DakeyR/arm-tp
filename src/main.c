#include <stdint.h>

#define GET_REGISTER_ADDR(base, offset) base + offset
#define GPIOA_BASE 0x40020000
#define IDR_OFFSET 0x10

void button_pressed(void)
{
    while (1)
        ;
}

void main(void) {

    uint32_t reg = GET_REGISTER_ADDR(GPIOA_BASE, IDR_OFFSET);
    while (1)
        if (reg & 0x0001)
            button_pressed();
}
