MEMORY {
  ccm (rw)      : org = 0x10000000, l = 64K
  sram1 (rwx)   : org = 0x20000000, l = 112K
  sram2 (rwx)   : org = 0x2001c000, l = 16K
  sram3 (rwx)   : org = 0x20020000, l = 64K
  flash (rwx)   : org = 0x08000000, l = 2M
  bkpsram (rwx) : org = 0x40024000, l = 4K

  ram (rwx)     : org = ORIGIN(sram1), l = LENGTH(sram1) + LENGTH(sram2) + LENGTH(sram3)
}

ENTRY(start)

stack_size = 2048;
heap_size = 1024;

_stack_start = ORIGIN(ram) + LENGTH(ram);
_stack_end = _stack_start - stack_size;


SECTIONS {
  /* Contains SP/PC and the interrupt handlers table */
  .isr_vector : {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
  } > flash

  .text : {
    . = ALIGN(4);
    *(.text)
    . = ALIGN(4);
  } > flash

  .data : {
    _data_start = .;
    *(.data);
    . = ALIGN(4);
    _data_end = .;
  } > ram AT > flash
  _data_loadaddr = LOADADDR(.data);

  .bss : {
    _bss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _bss_end = .;
  }

  .heap(NOLOAD) : {
    . = ALIGN(4);
    _heap_start = .;
    . = . + heap_size;
    _heap_end = .;
  } > ram
}
