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
	ld -o $@ -Ttext 0x10000 $^ --oformat binary

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) $(IFLAGS) -ffreestanding -c $< -o $@

%.o: %.asm
	$(ASM) $< -f elf64 -o $@

%.bin: %.asm
	$(ASM) $< -f bin -o $@

clean:
	rm -f *.bin *.o kernel/*.bin kernel/*.o boot/*.bin boot/*.o drivers/*.o

