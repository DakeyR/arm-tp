#include <stddef.h>

#include "board.h"
#include "interrupt_handlers.h"
#include "start.h"


const unsigned char _N_INTERRUPTS = N_INTERRUPTS; // Used in linker script

static
interrupt_callback *interrupt_callbacks[N_INTERRUPTS] = { 0 };


void handle_interrupt(irq_num_t n, const char *name)
{
    REG_SET_BIT(EXTI_PR, n);
    REG_SET_BIT(NVIC_ICPR0, n); // FIXME: NVIC_ICPR[x]

    interrupt_callback *callback = interrupt_callbacks[n];
    if (callback)
        callback(n);
}

static
void handle_exception(irq_num_t n, const char *name)
{
    for (;;)
        continue;
}


interrupt_callback* on_interrupt(irq_num_t n, interrupt_callback *callback)
{
    interrupt_callback *prev_callback = interrupt_callbacks[n];
    interrupt_callbacks[n] = callback;
    return prev_callback;
}


// Exceptions: common to all boards

EXCEPTION_HANDLER(0x1, reset, start);
EXCEPTION(0x2, nmi);
EXCEPTION(0x3, hard_fault);
EXCEPTION(0x4, mem_manage_fault);
EXCEPTION(0x5, bus_fault);
EXCEPTION(0x6, usage_fault);
EXCEPTION(0x7, reserved);
EXCEPTION(0x8, reserved);
EXCEPTION(0x9, reserved);
EXCEPTION(0xA, reserved);
EXCEPTION(0xB, svcall);
EXCEPTION(0xC, debug_monitor);
EXCEPTION(0xD, reserved);
EXCEPTION(0xE, pend_sv);
EXCEPTION(0xF, systick);
