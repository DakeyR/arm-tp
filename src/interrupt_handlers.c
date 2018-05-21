#include "board.h"
#include "interrupt_handlers.h"

void button_handler()
{
    REG_SET_BIT(EXTI_PR, 0x6);
    REG_SET_BIT(NVIC_ICPR0, 0x6);
    GET_REG(GPIO_G_ODR) ^= 0x1 << 13;
}
