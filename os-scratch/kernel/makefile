kernel_entry.o: kernel_entry.asm
	nasm kernel_entry.asm -f elf64 -o kernel_entry.o

kernel.o: kernel.c
	gcc -ffreestanding -c kernel.c -o kernel.o

kernel.bin: kernel_entry.o kernel.o
	ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

bootsect.bin: bootsect.asm
	nasm bootsect.asm -f bin -o bootsect.bin

os-image.bin: bootsect.bin kernel.bin
	cat bootsect.bin kernel.bin > os-image.bin

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

clean:
	rm *.bin *.o
