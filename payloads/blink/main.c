#include <stddef.h>
#include <stdint.h>


void main(void)
{
    volatile uint32_t *RCC_AHB1ENR = (void *)(0x40023800 + 0x30);
    volatile uint32_t *GPIO_G_MODER = (void *)(0x40021800 + 0x00);
    volatile uint32_t *GPIO_G_ODR = (void *)(0x40021800 + 0x14);

    // Enable GPIO G
    *RCC_AHB1ENR |= 1 << 6;

    // GPIOG write mode
    *GPIO_G_MODER |= 1 << 26;

    for (size_t i = 0;; i++)
        if (i % (1 << 16) == 0)
            *GPIO_G_ODR ^= 1 << 13; // Swap LED state
}
