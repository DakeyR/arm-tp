#include "board.h"
#include "interrupt_handlers.h"

void button_handler()
{
    GET_REG(GPIO_G_ODR) ^= 0x1 << 13;
}
