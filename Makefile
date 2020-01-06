ASM := asm/bin/asm
SIM := sim/bin/sim

all: asm sim	

asm:
	+$(MAKE) -C asm

sim:
	+$(MAKE) -C sim

example:
	@+$(MAKE) -s -C asm example
	@+$(MAKE) -s -C sim example

clean:
	+$(MAKE) -C asm clean
	+$(MAKE) -C sim clean

.PHONY: all asm sim example clean

