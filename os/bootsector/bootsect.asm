[org 0x7c00]

; initialize stack
mov bp, 0x8000
mov sp, bp

; set up registers to read from disk
mov bx, 0x9000    ; es:bx will be 0x0000:0x9000 = 0x09000, which is where the
                  ; disk contents will be read to
mov dh, 2         ; request that 2 sectors be read
call disk_load

mov bx, [0x9000]  ; print the first word that was read from disk so that we
                  ; can confirm it's working properly
call print_hex
call print_newline

mov bx, [0x9000 + 512]    ; print the first word that was read from the second
                          ; disk sector
call print_hex
call print_newline

jmp $

; ------ Data ------ ;

TEST:
  db "AAABBB", 0

; ------ Text ------ ;
%include "print.asm"
%include "disk_load.asm"

times 510 - ($-$$) db 0
dw 0xaa55

; additional sectors
times 256 dw 0xabcd
times 256 dw 0x1234

