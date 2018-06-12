MEMORY {
  ccm (rw)      : org = 0x10000000, l = 64K
  sram1 (rwx)   : org = 0x20000000, l = 112K
  sram2 (rwx)   : org = 0x2001c000, l = 16K
  sram3 (rwx)   : org = 0x20020000, l = 64K
  flash (rx)    : org = 0x08000000, l = 2M
  bkpsram (rwx) : org = 0x40024000, l = 4K

  ram (rwx)     : org = ORIGIN(sram1), l = LENGTH(sram1) + LENGTH(sram2) + LENGTH(sram3)
}

ENTRY(start)

PROVIDE(stack_size = 2048);
PROVIDE(heap_size = 1024);

_stack_start = ORIGIN(ram) + LENGTH(ram);
_stack_end = _stack_start - stack_size;


SECTIONS {
  .sp : {
    LONG(ORIGIN(ram) + LENGTH(ram))
  } > flash

  .isr_vector : {
    KEEP(*(SORT(.isr_vector.exceptions.*)))
    . = ALIGN(16 * 4);

    KEEP(*(SORT(.isr_vector.interrupts.*)))
    . = ALIGN(_N_INTERRUPTS * 4);
  } > flash

  .text : {
    . = ALIGN(4);
    *(.text.*)
    . = ALIGN(4);
  } > flash

  .data : {
    . = ALIGN(4);
    _data_start = .;
    *(.data.*);
    . = ALIGN(4);
    _data_end = .;
  } > ram AT > flash
  _data_loadaddr = LOADADDR(.data);

  .bss : {
    _bss_start = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _bss_end = .;
  } > ram

  .heap (NOLOAD) : {
    . = ALIGN(4);
    _heap_start = .;
    . = . + heap_size;
    _heap_end = .;
  } > ram
}
