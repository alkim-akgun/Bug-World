ASM := asm/bin/asm
SIM := sim/bin/sim

all: asm sim	

asm:
	$(MAKE) -C asm

sim:
	$(MAKE) -C sim

example:
	@$(MAKE) -s -C asm example
	@$(MAKE) -s -C sim example

clean:
	$(MAKE) -s -C asm cleanall
	$(MAKE) -s -C sim cleanall

.PHONY: all asm sim example clean

