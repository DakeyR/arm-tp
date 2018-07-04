PAYLOAD = $(shell basename $(shell pwd))


.PHONY: all clean flash


all:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) payload_all

clean:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) payload_clean

flash:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) payload_flash
