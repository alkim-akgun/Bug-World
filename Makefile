ASM := asm/bin/asm
SIM := sim/bin/sim

all: asm sim	

asm:
	$(MAKE) -C asm

sim:
	$(MAKE) -C sim

tour:
	$(MAKE) -C tour

example:
	$(MAKE) -s -C asm example
	$(MAKE) -s -C sim example
	$(MAKE) -s -C tour example

clean:
	$(MAKE) -s -C asm cleanall
	$(MAKE) -s -C sim cleanall
	$(MAKE) -s -C tour cleanall

.PHONY: all asm sim example clean

