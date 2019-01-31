#include "drivers/screen.h"
#include "kernel/shell.h"

// ???
void entrypoint() {
}

void main() {
  clear_terminal();
  kprint("Hello World!\n\n\n");
  kprint("HELLO WORLD!\n");
  init_shell();
  return;
}

