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
    REG_SET_BIT(RCC_APB2ENR, 4); // USART1 clock enable

    // GET_REG(USART1_BRR) = (/* mantissa */ 8 << 4) | (/* frac */ 11); // 115200 bps
    GET_REG(USART1_BRR) = (/* mantissa */ 416 << 4) | (/* frac */ 11); // 9600 bps

    REG_SET_BIT(USART1_CR1, 13); // USART1 enable

    const char buf[] = "Hello, World!\r\n";
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
