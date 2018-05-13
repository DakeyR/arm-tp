extern const void *_stack_start;

const void *_isr_vector[82] __attribute__((section (".isr_vector"))) = {
    &_stack_start,
    0x0
};


int start(void) {
	return 0;
}
