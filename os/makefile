CC=gcc
IFLAGS=-I .
CFLAGS=-g
ASM=nasm

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = $(C_SOURCES:.c=.o)

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) $(IFLAGS) -ffreestanding -m32 -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf -o $@

%.bin: %.asm
	$(ASM) $< -f bin -o $@

clean:
	rm -f *.bin *.o kernel/*.bin kernel/*.o boot/*.bin boot/*.o drivers/*.o

