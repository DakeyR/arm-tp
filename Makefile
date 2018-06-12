TOOLCHAIN = arm-none-eabi

CC = ${TOOLCHAIN}-gcc
LD = ${TOOLCHAIN}-ld

SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)


CFLAGS = -mcpu=cortex-m4 -nostartfiles -static -g
LDFLAGS = -T stm32.lds


# GC symbols
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -gc-sections # -print-gc-sections


.PHONY: all clean debug flash

all: stm32.bin

clean:
	$(RM) $(OBJS) stm32.out

flash: stm32.bin
	st-flash write $< 0x08000000

debug: stm32.elf
	@nohup st-util >/dev/null 2>&1 &
	@$(TOOLCHAIN)-gdb $< -quiet -ex 'target remote :4242' -ex 'b main'


stm32.elf: $(OBJS) stm32.lds
	$(LD) $(LDFLAGS) $(OBJS) -o $@

stm32.bin: stm32.elf
	$(TOOLCHAIN)-objcopy -O binary -S $< $@
