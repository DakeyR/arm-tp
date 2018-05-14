extern const void *_stack_start;

void start(void);


const void *_isr_vector[82] __attribute__((section (".isr_vector"))) = {
    &_stack_start,
    start,
    0x0
};


extern void main(void);

void start(void) {
    // TODO: memcpy(_data_start, _data_loadaddr, _data_end - _data_end);
    // TODO: memset(_bss_start, 0, _bss_end - _bss);

    main();

    // TODO: handle returning main
}
