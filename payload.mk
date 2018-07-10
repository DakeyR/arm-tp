PAYLOAD = $(shell basename $(shell pwd))


.PHONY: all clean debug flash


all:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) $@

clean:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) $@

debug:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) $@

flash:
	$(MAKE) -C ../.. PAYLOAD=$(PAYLOAD) $@
