#include <stdint.h>

#include "interrupt_handlers.h"


extern uint32_t _bss_start, _bss_end, _data_start, _data_end, _data_loadaddr;
extern void main(void);


void start(void)
{
    // memset(_bss_start, 0, _bss_end - _bss_start);
    for (uint32_t *bss = &_bss_start; bss < &_bss_end; bss++)
        *bss = 0;

    // memcpy(_data_start, _data_loadaddr, _data_end - _data_start);
    uint32_t *data_src = &_data_loadaddr;
    uint32_t *data_dst = &_data_start;
    while (data_dst < &_data_end)
        *(data_dst++) = *(data_src++);


    main();


    for (;;)
        asm volatile ("wfi");
}
