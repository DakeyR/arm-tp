#pragma once

#include <stdint.h>


#define N_INTERRUPTS 92


#define REG_NAME(Name) \
        _REG_ ##Name

#define DECLARE_REGISTER(Name) \
        extern volatile uint32_t *REG_NAME(Name)

#define DEFINE_REGISTER(Name, Base, Offset) \
        volatile uint32_t *REG_NAME(Name) = (uint32_t *)(Base + Offset)

#define GET_REGISTER(Name) *(REG_NAME(Name))
#define GET_REG(Name) GET_REGISTER(Name)

#define REG_GET_BIT(RegName, Bit) \
        ((GET_REG(RegName) & (1 << (Bit))) >> (Bit))

#define REG_SET_BIT(RegName, Bit) \
        GET_REG(RegName) |=   1 << (Bit)

#define REG_CLR_BIT(RegName, Bit) \
        GET_REG(RegName) &= ~(1 << (Bit))


#define DECLARE_GPIO(Name) \
        DECLARE_REGISTER(GPIO_ ##Name## _MODER  ); \
        DECLARE_REGISTER(GPIO_ ##Name## _OTYPER ); \
        DECLARE_REGISTER(GPIO_ ##Name## _OSPEEDR); \
        DECLARE_REGISTER(GPIO_ ##Name## _PUPDR  ); \
        DECLARE_REGISTER(GPIO_ ##Name## _IDR    ); \
        DECLARE_REGISTER(GPIO_ ##Name## _ODR    ); \
        DECLARE_REGISTER(GPIO_ ##Name## _BSRR   ); \
        DECLARE_REGISTER(GPIO_ ##Name## _LCKR   ); \
        DECLARE_REGISTER(GPIO_ ##Name## _AFRL   ); \
        DECLARE_REGISTER(GPIO_ ##Name## _AFRH   )

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


DECLARE_GPIO(A);
DECLARE_GPIO(B);
DECLARE_GPIO(C);
DECLARE_GPIO(D);
DECLARE_GPIO(E);
DECLARE_GPIO(F);
DECLARE_GPIO(G);
DECLARE_GPIO(H);
DECLARE_GPIO(I);
DECLARE_GPIO(J);
DECLARE_GPIO(K);


DECLARE_REGISTER(RCC_AHB1ENR);
DECLARE_REGISTER(RCC_APB2ENR);
DECLARE_REGISTER(SYSCFG_EXTICR1);
DECLARE_REGISTER(EXTI_IMR);
DECLARE_REGISTER(EXTI_FTSR);
DECLARE_REGISTER(EXTI_RTSR);
DECLARE_REGISTER(EXTI_PR);
DECLARE_REGISTER(NVIC_ISER0);
DECLARE_REGISTER(NVIC_ICPR0);


DECLARE_REGISTER(USART1_SR);
DECLARE_REGISTER(USART1_DR);
DECLARE_REGISTER(USART1_BRR);
DECLARE_REGISTER(USART1_CR1);
DECLARE_REGISTER(USART1_CR2);
DECLARE_REGISTER(USART1_CR3);
DECLARE_REGISTER(USART1_GTPR);
