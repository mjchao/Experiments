[org 0x7c00]

mov ah, 0x0e

mov bp, 0x1000 ; bp is the bottom of the stack, start it at mem address 0x1000
mov sp, bp     ; sp is the top of the stack (but as the stack expands, it's
               ; addresses gets smaller - e.g. the second element will be at
               ; 0x998

push '!'
push 'I'
push 'H'

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

mov al, [0xffe]   ; 0x1000 - 2
int 0x10

mov al, [0x1000 - 2]
int 0x10

pop bx
mov al, bl
int 0x10

jmp $
times 510-($-$$) db 0
dw 0xaa55

