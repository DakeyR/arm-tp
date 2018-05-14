#include <stdint.h>

extern const void *_stack_start;

void start(void);


const void *_isr_vector[82] __attribute__((section (".isr_vector"))) = {
    &_stack_start,
    start,
    0x0
};


extern uint32_t _bss_start, _bss_end, _data_start, _data_end, _data_loadaddr;
extern void main(void);

void start(void) {
    // memset(_bss_start, 0, _bss_end - _bss);
    for (uint32_t *bss = &_bss_start; bss < &_bss_end; bss++)
        *bss = 0;

    // memcpy(_data_start, _data_loadaddr, _data_end - _data_end);
    uint32_t *data_src = &_data_loadaddr;
    uint32_t *data_dst = &_data_loadaddr;
    while (data_src < &_data_loadaddr + ((int)&_data_end - (int)&_data_start))
        *(data_dst++) = *(data_src++);


    main();

    // TODO: handle returning main
}
