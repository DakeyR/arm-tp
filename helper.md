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