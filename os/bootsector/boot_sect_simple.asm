; Loop forever
loop:
  jmp loop

; Fill up to the first 510 bytes
times 510-($-$$) db 0

; Last two bytes are 0x55aa (little endian format, so we need to write 0xaa55)
; indicating the first 512 bytes were a boot sector
dw 0xaa55

