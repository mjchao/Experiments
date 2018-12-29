; Prints the character in the al register
print_al:
  pusha
  mov ah, 0x0e
  int 0x10
  popa
  ret

; Prints a null-terminated string pointed to by the bx register
print_str:
  pusha

print_str_loop:

  ; if char is \0, then we are done
  mov al, [bx]
  cmp al, 0
  je print_str_done

  ; if char is not \0, then print it
  mov al, [bx]
  call print_al

  ; move on to next character and repeat
  add bx, 1
  jmp print_str_loop

print_str_done:
  popa
  ret


; Prints the value in the bx register in hex
print_hex:
  pusha

  ; counter, we iterate 4 times and stop when cx = 4
  mov cx, 0

print_hex_loop:

  ; once we've processed with the 0x000f bitmask, the shift brings us back to 0
  cmp cx, 4
  je print_hex_done

  mov ax, bx     ; use a as working register
  and ax, 0xf000 ; mask out the first digit
  shr ax, 12     ; shift the first digit to the very left
  cmp al, 0x9    ; determine if we're printing ASCII 0-9 or A-Z
  jle print_hex_extract_0_to_9
  jmp print_hex_extract_a_to_z

print_hex_extract_0_to_9:
  add al, 0x30 ; '0' is ASCII 48 or ASCII 0x30
  jmp print_hex_char

print_hex_extract_a_to_z:
  sub al, 0xa  ; Subtract 0xa to re-index the A-F at 0
  add al, 0x41 ; 'A' is ASCII 65
  jmp print_hex_char

print_hex_char:
  call print_al
  rol bx, 4    ; rotate bits in bx, e.g. 0x1234 -> 0x2341
  add cx, 1
  jmp print_hex_loop

print_hex_done:
  popa
  ret

