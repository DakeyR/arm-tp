TOOLCHAIN = arm-none-eabi

CC = ${TOOLCHAIN}-gcc
LD = ${TOOLCHAIN}-ld

# Bootloader
BUILD_DIR = build

SRC = $(shell find src -name '*.c')

PROG_BIN = stm32.bin
PROG_ADDR = 0x08000000

LDSCRIPT = stm32.lds

CPPFLAGS = -iquote src

CFLAGS = -mcpu=cortex-m4 -nostartfiles -static -g
LDFLAGS = -T $(LDSCRIPT)

# GC symbols
CFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -gc-sections # -print-gc-sections


# Payloads
ifdef PAYLOAD

PAYLOAD_DIR = payloads/$(PAYLOAD)
BUILD_DIR = $(PAYLOAD_DIR)/build

SRC = $(PAYLOAD_DIR)/main.c
# if main.c doesn't exist
ifeq ($(wildcard $(SRC)),)
SRC = $(shell find $(PAYLOAD_DIR)/src -name '*.c')
endif

SRC += src/start.c src/board.c

PROG_BIN = $(PAYLOAD).bin
PROG_ADDR = 0x08004000

LDSCRIPT = payload.lds

CPPFLAGS = -I src -DPAYLOAD

CFLAGS = -mcpu=cortex-m4 -nostartfiles -static -g
LDFLAGS = -T $(LDSCRIPT)

# CFLAGS += -fpic -mpic-data-is-text-relative
endif


OBJS = $(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

PROG_BIN := $(BUILD_DIR)/$(PROG_BIN)
PROG_ELF = $(PROG_BIN:.bin=.elf)


.PHONY: all clean debug flash

all: $(PROG_BIN)

flash: $(PROG_BIN)
	st-flash write $< $(PROG_ADDR)

debug: $(PROG_ELF)
	@nohup st-util >/dev/null 2>&1 &
	@$(TOOLCHAIN)-gdb $< -quiet -ex 'target remote :4242' -ex 'b main'

clean:
	$(RM) $(OBJS) $(PROG_BIN) $(PROG_ELF)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(PROG_ELF): $(OBJS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(PROG_BIN): $(PROG_ELF)
	$(TOOLCHAIN)-objcopy -O binary -S $< $@
