#include <stddef.h>
#include <stdint.h>

#include "board.h"
#include "drivers/flash.h"
#include "drivers/uart.h"
#include "interrupt_handlers.h"


#define STR(X) #X
#define REFLASH_VERSION 1

typedef struct
{
    uint8_t version;
    uint32_t prog_len;
    uint32_t chunk_size;
} header_t;

void reflash_board_recv(void *buf, size_t n);
void reflash_board_fatal(const char *error_msg);
void reflash_board_main(void *payload_dst);

// Receive n bytes into buf via UART.
// The goal is to pair this function with a send on the PC side that sends
// a 32-bit checksum, and then the data, ensuring data integrity.
void reflash_board_recv(void *buf, size_t n)
{
    char *cbuf = buf;

    size_t try = 0;
    for (;;)
    {
        uart_error_t err;
        uart_recv(cbuf, n, 0, &err);

        int retry = err;
        if (!retry)
        {
            // TODO: Checksum
        }

        if (!retry)
            break;

        if (try++ == 5)
        {
            uart_send("end", 3);
            reflash_board_fatal("receiving failed 5 times in a row");
        }

        uart_send("err", 3);
    }

    uart_send("ack", 3);
}

// Send the error via UART in a way that the PC program can recognize, and then
// loop forever to "stop" execution on the board.
void reflash_board_fatal(const char *error_msg)
{
    size_t len = 0;
    while (error_msg[len])
        len++;

    const char buf[] = "ERROR: ";
    uart_send(buf, sizeof(buf));
    uart_send(error_msg, len - 1);

    for (;;)
        continue;
}

void reflash_board_main(void *payload_dst)
{
    header_t header;
    reflash_board_recv(&header, sizeof(header));

    if (header.version != REFLASH_VERSION)
        reflash_board_fatal("incomptaible versions (on board = " STR(REFLASH_VERSION) ")");

    char buf[header.chunk_size];

    for (size_t i = 0; i < header.prog_len / header.chunk_size; i++)
    {
        reflash_board_recv(&buf, sizeof(buf));
        flash_write(payload_dst, buf, sizeof(buf));

        payload_dst += sizeof(buf);
    }

    size_t extra = header.prog_len % header.chunk_size;
    if (extra != 0)
    {
        reflash_board_recv(&buf, extra);
        flash_write(payload_dst, buf, extra);
    }
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
    // REG_SET_BIT(GPIO_A_PUPDR, 19); // PA9 Pull-down

    REG_SET_BIT(GPIO_A_PUPDR, 20); // PA10 Pull-up
    // REG_SET_BIT(GPIO_A_PUPDR, 21); // PA10 Pull-down

    REG_SET_BIT(RCC_APB2ENR, 4); // USART1 clock enable

    GET_REG(USART1_BRR) = (/* mantissa */ 8 << 4) | (/* frac */ 11); // 115200 bps
    // GET_REG(USART1_BRR) = (/* mantissa */ 104 << 4) | (/* frac */ 3); // 9600 bps
    // GET_REG(USART1_BRR) = (/* mantissa */ 416 << 4) | (/* frac */ 11); // 2400 bps

    REG_SET_BIT(USART1_CR1, 13); // USART1 enable

#define TEST_UART 0
#if TEST_UART
    for (int i = 0; i < 100; i++)
        uart_send(".", 1);
    uart_send("\r\n", 2);

    while (1)
    {
        uart_error_t err;
        char c;
        uart_recv(&c, 1, 0, &err);

        if (err)
        {
            uart_send("\r\nERR!\r\n", 8);
            break;
        }

        uart_send("-> ", 3);
        uart_send(&c, 1);
        uart_send("\r\n", 2);
    }
#else
    // FIXME: board.h: declare array with sectors
    void *flash_sect_1 = (void *)0x08004000;

    void *payload_dst = flash_sect_1;

    // TODO: add timeout to recv
    // if not timeout: recv program and reflash
    // jump to flash_sect_1
    char reflash_over_uart;
    uart_error_t err;
    uart_recv(&reflash_over_uart, 1, 1024, &err);
    if (err & UART_ERR_TIMEOUT == 0)
        reflash_board_main(payload_dst);

    // GPIO_ENABLE(G);

    // // GPIOG write mode
    // GPIO_SET_PIN_MODE(G, 13, GPIO_PIN_MODE_OUTPUT);

    // + 1: 16-bit mode
    void (*payload_start)(void) = (void *)((char *)payload_dst + 1);
    payload_start();
#endif
}
