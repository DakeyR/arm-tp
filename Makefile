TOOLCHAIN = arm-none-eabi

CC = ${TOOLCHAIN}-gcc
LD = ${TOOLCHAIN}-ld

SRC = $(shell find src -name '*.c')
OBJS = $(SRC:.c=.o)

CPPFLAGS = -iquote src

CFLAGS = -mcpu=cortex-m4 -nostartfiles -static -g
LDFLAGS = -T stm32.lds


# GC symbols
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -gc-sections # -print-gc-sections


.PHONY: all clean debug flash payload_all payload_clean

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


# Makefile for payloads:
ifdef PAYLOAD

PAYLOAD_DIR = payloads/$(PAYLOAD)

SRC = $(PAYLOAD_DIR)/main.c
# if main.c doesn't exist
ifeq ($(wildcard $(SRC)),)
SRC = $(shell find $(PAYLOAD_DIR)/src -name '*.c')
endif

OBJS = $(SRC:.c=.o)

PAYLOAD_BIN = $(PAYLOAD_DIR)/$(PAYLOAD).bin
PAYLOAD_ELF = $(PAYLOAD_DIR)/$(PAYLOAD).elf


CPPFLAGS = -I src

CFLAGS = -mcpu=cortex-m4 -nostartfiles -static -g
LDFLAGS = -T payload.lds


payload_all: $(PAYLOAD_BIN)

payload_clean:
	$(RM) $(OBJS) $(PAYLOAD_BIN) $(PAYLOAD_ELF)

payload_flash: $(PAYLOAD_BIN)
	st-flash write $(PAYLOAD_BIN) 0x08004000


$(PAYLOAD_ELF): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(PAYLOAD_BIN): $(PAYLOAD_ELF)
	$(TOOLCHAIN)-objcopy -O binary -S $< $@

endif
