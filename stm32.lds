MEMORY {
  ccm (rw)      : org = 0x10000000, l = 64K
  sram1 (rwx)   : org = 0x20000000, l = 112K
  sram2 (rwx)   : org = 0x2001c000, l = 16K
  sram3 (rwx)   : org = 0x20020000, l = 64K
  flash (rwx)   : org = 0x08000000, l = 2M
  bkpsram (rwx) : org = 0x40024000, l = 4K
}

ENTRY(main)

SECTIONS {
  /* Contains SP/PC and the interrupt handlers table */
  .isr_vector : {
    KEEP(*(.isr_vector))
  } > flash

  .text : {
    . = ALIGN(4);
    *(.text)
    . = ALIGN(4);
  } > flash
}
