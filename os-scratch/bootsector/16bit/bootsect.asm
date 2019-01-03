[org 0x7c00]

; initialize stack
mov bp, 0x8000
mov sp, bp

; set up registers to read from disk
mov bx, 0x9000    ; es:bx will be 0x0000:0x9000 = 0x09000, which is where the
                  ; disk contents will be read to
mov dh, 2         ; request that 2 sectors be read
call disk_load

; check for errors

; disk error:
mov bl, 0x01
cmp al, bl
je bootsect_disk_error

; sectors error:
mov bl, 0x02
cmp al, bl
je bootsect_sectors_error

; no error:
jmp bootsect_disk_load_success

; Displays an error message informing the user that the BIOS could not read
; the disk
bootsect_disk_error:
  mov bx, DISK_ERROR
  call print_str
  mov bh, 0x00        ; print BIOS error code
  mov bl, ah
  call print_hex
  call print_newline
  jmp bootsect_spin

; Displays an error message informing the user that the BIOS could not read
; the sectors
bootsect_sectors_error:
  mov bx, SECTORS_ERROR
  call print_str
  call print_newline
  jmp bootsect_spin

; Logic for when the disk is loaded successfully
bootsect_disk_load_success:
  mov bx, [0x9000]  ; print the first word that was read from disk so that we
                    ; can confirm it's working properly
  call print_hex
  call print_newline

  mov bx, [0x9000 + 512]    ; print the first word that was read from the second
                            ; disk sector
  call print_hex
  call print_newline
  jmp bootsect_spin

bootsect_spin:
  jmp $

; ------ Data ------ ;

DISK_ERROR:
  db "Disk read error. BIOS Error Code: ", 0

SECTORS_ERROR:
  db "Incorrect number of sectors read", 0

; ------ Text ------ ;
%include "print.asm"
%include "disk_load.asm"

times 510 - ($-$$) db 0
dw 0xaa55

; additional sectors
times 256 dw 0xabcd
times 256 dw 0x1234

