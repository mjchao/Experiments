[org 0x7c00]

mov al, '!'
call print_al

call print_NEWLINE

call print_HELLO

call print_NEWLINE

call print_GOODBYE

call print_NEWLINE

jmp $

; ------- Data -------- ;
HELLO:
  db 'Hello, World!!!!', 0

NEWLINE:
  db 0x0d, 0x0a, 0

GOODBYE:
  db 'GOODBYE!!!!', 0

; ------- Text ------------ ;

; Prints a character in the al register
print_al:
  pusha
  mov ah, 0x0e
  int 0x10     ; assumes character to print already in al
  popa
  ret

; Prints a string in bx register
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

print_HELLO:
  pusha 
  mov bx, HELLO
  call print_str
  popa
  ret

print_NEWLINE:
  pusha
  mov bx, NEWLINE
  call print_str
  popa
  ret

print_GOODBYE:
  pusha
  mov bx, GOODBYE
  call print_str
  popa
  ret 

; ------ Padding and Magic Number ------- ;
times 510-($-$$) db 0
dw 0xaa55

