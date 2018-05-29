#include <stdint.h>

#include "board.h"
#include "interrupt_handlers.h"


void button_pressed(irq_num_t n)
{
    GET_REG(GPIO_G_ODR) ^= 1 << 13;
}


void main(void)
{
    // TODO: Generic interrupt enabling
    GET_REG(SYSCFG_EXTICR1) |=  0x0000;
    GET_REG(EXTI_IMR) |= 0x1;
    GET_REG(EXTI_FTSR) |= 0x1;
    GET_REG(EXTI_RTSR) |= 0x1;

    REG_SET_BIT(NVIC_ISER0, 0x6);

    __asm__ __volatile__ ("cpsie i");


    GPIO_ENABLE(G);
    GPIO_SET_PIN_MODE(G, 13, GPIO_PIN_MODE_OUTPUT);

    on_interrupt(6, button_pressed);

    while (1)
        continue;
}
