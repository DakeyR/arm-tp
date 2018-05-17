#include <stdint.h>


#define GET_REGISTER_ADDR(base, offset) *(base + offset)
#define DECLARE_REGISTER(name, base, offset) \
        volatile uint32_t *name = (uint32_t *)(base + offset)
#define GET_REGISTER(name) *(name)

//volatile const uint32_t *GPIOA_BASE = 0x40020000;
//const int IDR_OFFSET = 0x10;


DECLARE_REGISTER(gpio_a_idr, 0x40020000, 0x10);

DECLARE_REGISTER(gpio_g_odr, 0x40021800, 0x14);
DECLARE_REGISTER(gpio_g_moder, 0x40021800, 0x00);

DECLARE_REGISTER(rcc_ahb1enr, 0x40023800, 0x30);


void button_pressed(void)
{
    while (1)
        ;
}

void main(void)
{
    GET_REGISTER(rcc_ahb1enr) |= (0x1 << 0) | (0x1 << 6);  // GPIOA + GPIOG

    // GPIOG write mode
    GET_REGISTER(gpio_g_moder) |= 0x1 << 26;

    // Turn on LED
    GET_REGISTER(gpio_g_odr) |= 0x1 << 13;

    while (1)
        if (GET_REGISTER(gpio_a_idr) & 0x0001)
            button_pressed();
}
