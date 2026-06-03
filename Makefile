all:
	mkdir -p build
	nasm -f elf32 src/boot.asm -o build/boot.o
	gcc -m32 -c src/kernel.c -o build/kernel.o -ffreestanding
	ld -m elf_i386 -T linker.ld -o build/mykernel.bin build/boot.o build/kernel.o

run: all
	qemu-system-i386 -kernel build/mykernel.bin