#include <stdint.h>

#include "board.h"
#include "uart.h"


size_t uart_recv(char *buf, size_t max, uint8_t *error)
{
    size_t n = 0;

    if (error)
        *error = 0;

    REG_SET_BIT(USART1_CR1, 2); // RE: Receiver enable

    for (; n < max; n++)
    {
        while (!REG_GET_BIT(USART1_SR, 5)) // RXNE: Read data register not empty
            continue;

        *(buf)++ = GET_REG(USART1_DR);

        uint32_t status = GET_REG(USART1_SR);

        if (status & (1 << 4)) // IDLE
            break;

        if (error)
        {
            if (status & (1 << UART_ERR_PARITY))
                *error |= UART_ERR_PARITY;
            if (status & (1 << UART_ERR_FRAMING))
                *error |= UART_ERR_FRAMING;
            if (status & (1 << UART_ERR_NOISE))
                *error |= UART_ERR_NOISE;
            if (status & (1 << UART_ERR_OVERRUN))
                *error |= UART_ERR_OVERRUN;

            if (*error)
                break;
        }
    }

    REG_CLR_BIT(USART1_CR1, 2); // RE

    return n;
}


void uart_send(const char *buf, size_t n)
{
    REG_SET_BIT(USART1_CR1, 3); // TE: Transmitter enable

    while (n--)
    {
        GET_REG(USART1_DR) = *(buf++);

        while (REG_GET_BIT(USART1_SR, 7)) // TXE: Transmit data register empty
            continue;
    }

    while (!REG_GET_BIT(USART1_SR, 6)) // TC: Transmission complete
        continue;

    REG_CLR_BIT(USART1_CR1, 3); // TE
}
