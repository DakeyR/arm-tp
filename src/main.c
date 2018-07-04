#include <stdint.h>

#include "board.h"
#include "interrupt_handlers.h"
#include "uart.h"


void button_pressed(irq_num_t n)
{
    GET_REG(GPIO_G_ODR) ^= 1 << 13;
}


void main(void)
{
    GPIO_ENABLE(A);

    GPIO_SET_PIN_MODE(A,  9, GPIO_PIN_MODE_ALTERNATE);
    GPIO_SET_PIN_MODE(A, 10, GPIO_PIN_MODE_ALTERNATE);

    // Set PA9 and PA10 to alternate mode AF7
    GET_REG(GPIO_A_AFRH) |= 7 << (( 9 - 8) * 4);
    GET_REG(GPIO_A_AFRH) |= 7 << ((10 - 8) * 4);

    // REG_SET_BIT(GPIO_A_PUPDR, 18); // PA9 Pull-up
    REG_SET_BIT(GPIO_A_PUPDR, 19); // PA9 Pull-down
    REG_SET_BIT(GPIO_A_PUPDR, 20); // PA10 Pull-up
    // REG_SET_BIT(GPIO_A_PUPDR, 21); // PA10 Pull-down

    REG_SET_BIT(RCC_APB2ENR, 4); // USART1 clock enable

    // GET_REG(USART1_BRR) = (/* mantissa */ 8 << 4) | (/* frac */ 11); // 115200 bps
    // GET_REG(USART1_BRR) = (/* mantissa */ 104 << 4) | (/* frac */ 3); // 9600 bps
    GET_REG(USART1_BRR) = (/* mantissa */ 416 << 4) | (/* frac */ 11); // 2400 bps

    REG_SET_BIT(USART1_CR1, 13); // USART1 enable

    const char buf[] = "Hello, World!\r\n";
    while (1)
        uart_send(buf, sizeof(buf));

    while (1)
        continue;

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

    while (1)
        continue;
}
