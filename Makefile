CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)


.PHONY: all

all: stm32.out

stm32.out: $(OBJS)
	$(LD) -T stm32.lds $(OBJS) -o $@
