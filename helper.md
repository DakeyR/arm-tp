# ARM

## TOOLS

- arm-none-eabi-gcc 4.9
- st link utility

- STM 32 F4 Reference Manual
- STM 32 F429 Discovery Datasheet

- objdump : verifier l'adresse des sections
- nm : verifier l'addresse du main

## ELF

sp -> pointe vers le debut de la sram
pc -> pointe vers main

> un cpu ne sait pas executer un ELF

Transformer un elf en binaire  :

.ELF {
      .code
      .debug
      .data
      ...
}

> On peut créer ses propres sections dans un ELF

`ld -T ld.lds`

linker script
ld.lds: MEMORY (où + taille)
          ram/flash/bkram/ccm/sdram(FMC)
        SECTIONS

1. décrire le CPU dans un linker script
2. écrire le main + compiler
  -> devrait être au début de la flash
3. Mettre SP/PC/Vector d'handler d'exception au début de la mémoire (et pas le main) :
  -> être sur que le main n'empiète pas sur l'espace pour les handlers
d'exceptions.

`arm-none-eabi-gcc -Os  -ffunction-sections -mcpu=cortex-m4 -mfloat-abi=hard -mapcs -DSTM32F401xE -nostartfiles -T path/to/LinkerScript.ld -Wl,-gc-sections`

`-ffunction-sections` -> une section par fonction `.text.*` -> `-gc-section` peut donc virer les fonctions non-appelées

`arm-none-eabi-objcopy -O binary -S obj/startup.elf obj/startup.bin`

## Link

`st info --probe` -> infos sur le device
`st flash` -> flasher
  `st-flash write mon.bin 0x08000000`
`st util` -> pour se connecter + gdb (`arm-none-eabi-gdb`)

## GPIO

Par défaut, tout est en mode reset, les GPIOs sont désactivés.
-> il faut les activer avec le RCC (Reset...)

- Reference manual tells on which port what is plugged
- One pin can be configured to be used in read or write mode (input or output)
  - By default it is in read mode (input)
  - To read data -> GPIOx_IDR
  - To write data -> GPIOx_ODR
- To access the GPIO's register:
  - Memory map tells the base address for the GPIOx registers
  - Register maps tells the offset from the base address.

Presh button -> PA0 (GPIOA_IDR0 to get the state)
- Base Addr: 0x4002 0000
- Limit : 0x4002 03FF
- IDR: Offset = 0x10

## UART

0x4001 1000 - 0x4001 13FF -> USART1 -> PA9 (TX) - PA10 (RX) (Bus APB2)

Section 30.6.8: USART register map on page 1021

0x4001 1400 - 0x4001 17FF -> USART6
0x4000 4C00 - 0x4000 4FFF -> UART4
0x4000 7C00 - 0x4000 7FFF -> UART8
0x4000 7800 - 0x4000 7BFF -> UART7

## Clock

By default, HSI is ON (in RCC_CR)
We need to enable USART1 in RCC_APBENR
