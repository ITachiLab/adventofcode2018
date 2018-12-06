NASM = /usr/bin/nasm

main: main.o
	$(LD) -m elf_i386 $< -o $@

main.o: main.asm
	$(NASM) -f elf32 $< -o $@

