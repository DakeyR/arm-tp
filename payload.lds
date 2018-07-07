MEMORY {
  ccm (rw)      : org = 0x10000000, l = 64K
  sram1 (rwx)   : org = 0x20000000, l = 112K
  sram2 (rwx)   : org = 0x2001c000, l = 16K
  sram3 (rwx)   : org = 0x20020000, l = 64K
  flash (rx)    : org = 0x08000000, l = 2M
  bkpsram (rwx) : org = 0x40024000, l = 4K

  ram (rwx)     : org = ORIGIN(sram1), l = LENGTH(sram1) + LENGTH(sram2) + LENGTH(sram3)

  flash_sect_1 (rwx)    : org = ORIGIN(flash) + 0x4000, l = LENGTH(flash) - 0x4000
}

ENTRY(start)

SECTIONS {
  .text : {
    . = ALIGN(4);
    *(.text.*);
    . = ALIGN(4);
  } > flash_sect_1

  .data : {
    . = ALIGN(4);
    _data_start = .;
    *(.data.*);
    . = ALIGN(4);
    _data_end = .;
  } > ram AT > flash_sect_1
  _data_loadaddr = LOADADDR(.data);

  .bss : {
    _bss_start = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _bss_end = .;
  } > ram
}
