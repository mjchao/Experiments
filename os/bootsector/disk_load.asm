; loads 'dh' sectors from drive 'dl' into ES:BX
disk_load:
  pusha
  push dx

  mov ah, 0x02    ; ah is set to 0x02 to read disk sectors
  mov al, dh      ; al is set to number of disk sectors to read
  mov ch, 0x00    ; ch is the track/cylinder number
  mov cl, 0x02    ; cl is the sector number to read. 0x01 will be the boot
                  ; sector, so 0x02 will be the first available sector to read
  mov dh, 0x00    ; dh is the head number
  ; dl will be the drive number that the caller sets

  int 0x13        ; BIOS interrupt that will read disk sectors
  jc disk_error   ; if error, it will be stored in the carry bit

  pop dx
  cmp al, dh      ; BIOS will return number of sectors read in al. If it's not
                  ; equal to the number of sectors the caller wanted to read,
                  ; then there was an error reading the sectors.
  jne sectors_error
  popa
  ret

disk_error:
  mov bx, DISK_ERROR
  call print_str
  call print_newline
  mov bh, ah      ; ah contains the error code
  mov bl, 0x00    ; set lower bits to 0 because they're irrelevant
  call print_hex
  jmp disk_loop

sectors_error:
  mov bx, SECTORS_ERROR
  call print_str
  call print_newline

disk_loop:
  jmp $

DISK_ERROR:
  db "Disk read error", 0

SECTORS_ERROR:
  db "Incorrect number of sectors read", 0

