#include "drivers/screen.h"
#include "kernel/shell.h"

// ???
void entrypoint() {
}

void main() {
  shell_init();
  shell_kprint("Hello World!\n\n\n");
  shell_kprint("HELLO WORLD!\n");
  return;
}

