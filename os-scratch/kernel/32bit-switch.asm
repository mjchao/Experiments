[bits 16]
switch_to_pm:
  cli      ; disable interrupts which don't translate from 16-bit to 32-bit
  lgdt [gdt_descriptor]
  mov eax, cr0
  or eax, 0x1              ; set 32-bit mode in cr0
  mov cr0, eax
  jmp CODE_SEG:init_pm     ; jump to code segment that will start protected mode

[bits 32]
init_pm:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000           ; initialize stack
  mov esp, ebp

  call BEGIN_PM

