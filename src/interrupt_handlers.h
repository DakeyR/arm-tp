#pragma once


typedef unsigned char irq_num_t;
typedef void interrupt_callback(irq_num_t n);

interrupt_callback* on_interrupt(irq_num_t n, interrupt_callback callback);


void handle_interrupt(irq_num_t n, const char* name);


typedef void interrupt_service_routine(void);

#define ISR_VECTOR_ENTRY(ISR_Type, N, Name, Handler, Section) \
    static                                                                  \
    interrupt_service_routine* isr_vector_entry_##ISR_Type##_##N##_##Name   \
        __attribute__((section(Section "." #N)))                            \
            = Handler

#define HANDLER_AND_VECTOR_ENTRY(ISR_Type, N, Name, Section) \
    static                                                                          \
    void handle_##ISR_Type##_##N##_##Name(void)                                     \
    {                                                                               \
        handle_##ISR_Type(N, #Name);                                                \
    }                                                                               \
                                                                                    \
    ISR_VECTOR_ENTRY(ISR_Type, N, Name, handle_##ISR_Type##_##N##_##Name, Section)


#define EXCEPTION(N, Name) \
    HANDLER_AND_VECTOR_ENTRY(exception, N, Name, ".isr_vector.exceptions")

#define EXCEPTION_HANDLER(N, Name, Handler) \
    ISR_VECTOR_ENTRY(exception, N, Name, Handler, ".isr_vector.exceptions")


#define INTERRUPT(N, Name) \
    HANDLER_AND_VECTOR_ENTRY(interrupt, N, Name, ".isr_vector.interrupts")
