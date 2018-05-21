#pragma once
#include <stdint.h>

#define REG_NAME(Name) \
        _REG_ ##Name

#define DECLARE_REGISTER(Name, Base, Offset) \
        extern volatile uint32_t *REG_NAME(Name)

#define DEFINE_REGISTER(Name, Base, Offset) \
        volatile uint32_t *REG_NAME(Name) = (uint32_t *)(Base + Offset)

#define GET_REGISTER(Name) *(REG_NAME(Name))
#define GET_REG(Name) GET_REGISTER(Name)

#define REG_SET_BIT(RegName, Bit) \
        GET_REG(RegName) |=   1 << (Bit)

#define REG_CLR_BIT(RegName, Bit) \
        GET_REG(RegName) &= ~(1 << (Bit))


#define DECLARE_GPIO(Name, Base) \
        DECLARE_REGISTER(GPIO_ ##Name## _MODER,   Base, 0x00); \
        DECLARE_REGISTER(GPIO_ ##Name## _OTYPER,  Base, 0x04); \
        DECLARE_REGISTER(GPIO_ ##Name## _OSPEEDR, Base, 0x08); \
        DECLARE_REGISTER(GPIO_ ##Name## _PUPDR,   Base, 0x0C); \
        DECLARE_REGISTER(GPIO_ ##Name## _IDR,     Base, 0x10); \
        DECLARE_REGISTER(GPIO_ ##Name## _ODR,     Base, 0x14); \
        DECLARE_REGISTER(GPIO_ ##Name## _BSRR,    Base, 0x18); \
        DECLARE_REGISTER(GPIO_ ##Name## _LCKR,    Base, 0x1C); \
        DECLARE_REGISTER(GPIO_ ##Name## _AFRL,    Base, 0x20); \
        DECLARE_REGISTER(GPIO_ ##Name## _AFRH,    Base, 0x24)

#define DEFINE_GPIO(Name, Base) \
        DEFINE_REGISTER(GPIO_ ##Name## _MODER,   Base, 0x00); \
        DEFINE_REGISTER(GPIO_ ##Name## _OTYPER,  Base, 0x04); \
        DEFINE_REGISTER(GPIO_ ##Name## _OSPEEDR, Base, 0x08); \
        DEFINE_REGISTER(GPIO_ ##Name## _PUPDR,   Base, 0x0C); \
        DEFINE_REGISTER(GPIO_ ##Name## _IDR,     Base, 0x10); \
        DEFINE_REGISTER(GPIO_ ##Name## _ODR,     Base, 0x14); \
        DEFINE_REGISTER(GPIO_ ##Name## _BSRR,    Base, 0x18); \
        DEFINE_REGISTER(GPIO_ ##Name## _LCKR,    Base, 0x1C); \
        DEFINE_REGISTER(GPIO_ ##Name## _AFRL,    Base, 0x20); \
        DEFINE_REGISTER(GPIO_ ##Name## _AFRH,    Base, 0x24)

#define GPIO_ENABLE(Name) \
        REG_SET_BIT(RCC_AHB1ENR, ((#Name)[0] - 'A'))

#define GPIO_DISABLE(Name) \
        REG_CLR_BIT(RCC_AHB1ENR, ((#Name)[0] - 'A'))

#define GPIO_ENABLE_LP(Name) \
        REG_SET_BIT(RCC_AHB1LPENR, ((#Name)[0] - 'A'))

#define GPIO_DISABLE_LP(Name) \
        REG_CLR_BIT(RCC_AHB1LPENR, ((#Name)[0] - 'A'))


#define GPIO_SET_PIN_MODE(Name, Pin, Mode) \
        GET_REG(GPIO_ ##Name## _MODER) |= ((Mode) << (2 * (Pin)));

#define GPIO_PIN_MODE_INPUT     0
#define GPIO_PIN_MODE_OUTPUT    1
#define GPIO_PIN_MODE_ALTERNATE 2
#define GPIO_PIN_MODE_ANALOG    3

DECLARE_GPIO(A, 0x40020000);
DECLARE_GPIO(B, 0x40020400);
DECLARE_GPIO(C, 0x40020800);
DECLARE_GPIO(D, 0x40020C00);
DECLARE_GPIO(E, 0x40021000);
DECLARE_GPIO(F, 0x40021400);
DECLARE_GPIO(G, 0x40021800);
DECLARE_GPIO(H, 0x40021C00);
DECLARE_GPIO(I, 0x40022000);
DECLARE_GPIO(J, 0x40022400);
DECLARE_GPIO(K, 0x40022800);


DECLARE_REGISTER(RCC_AHB1ENR, 0x40023800, 0x30);
DECLARE_REGISTER(SYSCFG_EXTICR1, 0x40013800, 0x08);
DECLARE_REGISTER(EXTI_IMR, 0x40013c00, 0x00);
DECLARE_REGISTER(EXTI_FTSR, 0x40013c00, 0x08);
DECLARE_REGISTER(EXTI_RTSR, 0x40013c00, 0x0C);
DECLARE_REGISTER(EXTI_PR, 0x40013c00, 0x14);
DECLARE_REGISTER(NVIC_ISER0, 0xE000E100, 0x00);
DECLARE_REGISTER(NVIC_ICPR0, 0xE000E280, 0x00);
