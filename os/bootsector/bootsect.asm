mov ah, 0x0e ; tty mode
mov al, 'H'
int 0x10     ; write 'H' to display
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10
mov al, 'o'
int 0x10
mov al, ' '
int 0x10
mov al, 'W'
int 0x10
mov al, 'o'
int 0x10
mov al, 'r'
int 0x10
mov al, 'l'
int 0x10
mov al, 'd'
int 0x10
mov al, '!'
int 0x10

jmp $ ; Loop forever

; Fill up to the first 510 bytes
times 510-($-$$) db 0

; Last two bytes are 0x55aa (little endian format, so we need to write 0xaa55)
; indicating the first 512 bytes were a boot sector
dw 0xaa55

