#include "drivers/screen.h"
#include "cpu/isr.h"
#include "cpu/timer.h"

// ???
void entrypoint() {
}

void main() {
  clear_terminal();
  print_str("Hello World!\n\n\n", WHITE_ON_BLACK);
  print_str("HELLO WORLD!\n", WHITE_ON_BLACK);

  isr_install();
  //__asm__ __volatile__("int $2");
  //__asm__ __volatile__("int $3");
  asm volatile("sti");
  //init_timer(50);
  
  init_keyboard();
  return;
}

