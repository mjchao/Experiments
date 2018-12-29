[org 0x7c00]

; initialize stack
mov bp, 0x8000
mov sp, bp

mov bx, 0xabcd
call print_hex

jmp $

; ------ Data ------ ;

TEST:
  db "AAABBB", 0

; ------ Text ------ ;
%include "print.asm"

times 510 - ($-$$) db 0
dw 0xaa55

; additional sectors
times 256 dw 0xdddd
times 256 dw 0xffff

