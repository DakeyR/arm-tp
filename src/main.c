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
    REG_CLR_BIT(SYSCFG_EXTICR1, 0);
    REG_SET_BIT(EXTI_IMR, 0);
    REG_SET_BIT(EXTI_FTSR, 0);
    REG_SET_BIT(EXTI_RTSR, 0);

    REG_SET_BIT(NVIC_ISER0, 6);

    __asm__ __volatile__ ("cpsie i");


    GPIO_ENABLE(G);
    GPIO_SET_PIN_MODE(G, 13, GPIO_PIN_MODE_OUTPUT);

    on_interrupt(6, button_pressed);

    GET_REG(RCC_APB2ENR) |= 0x10;

    while (1)
        continue;
}
