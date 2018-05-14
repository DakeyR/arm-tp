TOOLCHAIN = arm-none-eabi

CC = ${TOOLCHAIN}-gcc
LD = ${TOOLCHAIN}-ld

SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)


CFLAGS = -ffunction-sections  -mcpu=cortex-m4 -nostartfiles -g #, -gc-sections -Map=obj.map
LDFLAGS = -T stm32.lds -gc-sections

.PHONY: all clean

all: stm32.bin

clean:
	$(RM) $(OBJS) stm32.out

stm32.elf: $(OBJS) stm32.lds
	$(LD) $(LDFLAGS) $(OBJS) -o $@

stm32.bin: stm32.elf
	$(TOOLCHAIN)-objcopy -O binary -S $< $@
