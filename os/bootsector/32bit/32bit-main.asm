[org 0x7c00]

mov bp, 0x9000        ; initialize stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print_str

call switch_to_pm
jmp $

%include "print16bit.asm"
%include "print32bit.asm"
%include "gdt.asm"
%include "32bit-switch.asm"

[bits 32]
BEGIN_PM:
  mov ebx, MSG_PROT_MODE
  call print_str_32bit
  jmp $

MSG_REAL_MODE:
  db "Started in 16-bit real mode", 0

MSG_PROT_MODE:
  db "Loaded 32-bit protected mode", 0

; bootsector
times 510 - ($-$$) db 0
dw 0xaa55

