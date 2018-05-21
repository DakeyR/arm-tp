#include <stdint.h>

#include "board.h"


void button_pressed(void)
{
    while (1)
        ;
}

void main(void)
{
    GPIO_ENABLE(A);
    GPIO_ENABLE(G);

    GPIO_SET_PIN_MODE(G, 13, GPIO_PIN_MODE_OUTPUT);

    // Turn on LED
    GET_REG(GPIO_G_ODR) |= 0x1 << 13;

    while (1)
        if (GET_REG(GPIO_A_IDR) & 0x0001)
            button_pressed();
}
