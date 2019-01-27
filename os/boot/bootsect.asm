[org 0x7c00]
KERNEL_OFFSET equ 0x1000       ; Linker will offset kernel code by 0x1000

  mov [BOOT_DRIVE], dl;
  mov bp, 0x9000
  mov sp, bp

  mov bx, MSG_REAL_MODE
  call print_str
  call print_newline

  call load_kernel             ; Read kernel from disk
  call switch_to_pm            ; enter 32-bit protected mode
  jmp $                        ; never executed
  

%include "boot/print16bit.asm"
%include "boot/print32bit.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/32bit_switch.asm"


[bits 16]
load_kernel:
  mov bx, MSG_LOAD_KERNEL
  call print_str
  call print_newline
  
  mov bx, KERNEL_OFFSET
  mov dh, 32
  mov dl, [BOOT_DRIVE]
  call disk_load
  ret

[bits 32]
BEGIN_PM:
  mov ebx, MSG_PROT_MODE
  call print_str_32bit
  call KERNEL_OFFSET       ; Give control to kernel
  jmp $                    ; Loop forever if kernel gives control back to us


BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Landed in 32-bit protected mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0


; padding
times 510 - ($-$$) db 0
dw 0xaa55

