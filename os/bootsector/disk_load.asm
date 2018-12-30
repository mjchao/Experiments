; loads 'dh' sectors from drive 'dl' into ES:BX
;
; There are two potenial BIOS errors: "Disk error" and "Sectors error".
;
; Disk error occurs if the BIOS could not read from the disk. al will be set
; to 0x1 and ah will be set to the BIOS error code.
;
; Sectors error occurs if the BIOS could not read the requested number
; of sectors. An error message will and al will be set to 0x2. If no error
; occurs, al will be set to 0x0.
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

  mov al, 0x00    ; indicate no error occurred.
  jmp disk_load_done

disk_error:
  mov al, 0x01    ; indicate a disk error occurred
  jmp disk_load_done

sectors_error:
  mov al, 0x02    ; indicate a sector error occurred
  jmp disk_load_done

disk_load_done:
  popa
  ret

