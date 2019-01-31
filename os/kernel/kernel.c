#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include "cpu/timer.h"

// ???
void entrypoint() {
}

void main() {
  clear_terminal();
  kprint("Hello World!\n\n\n");
  kprint("HELLO WORLD!\n");

  isr_install();
  //__asm__ __volatile__("int $2");
  //__asm__ __volatile__("int $3");
  asm volatile("sti");
  //init_timer(50);
  
  init_keyboard();
  return;
}

