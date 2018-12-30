[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f    ; color byte for each character

print_str_32bit:
  pusha
  mov edx, VIDEO_MEMORY

print_string_32bit_loop:
  mov al, [ebx]
  mov ah, WHITE_ON_BLACK

  cmp al, 0
  je print_str_32bit_done

  mov [edx], ax            ; store character + color in video memory
  add ebx, 1               ; move to next char
  add edx, 2               ; move to next video memory position
  jmp print_string_32bit_loop

print_str_32bit_done:
  popa
  ret

