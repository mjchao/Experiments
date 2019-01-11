#include "drivers/screen.h"

// ???
void entrypoint() {
}

void main() {
  clear_terminal();
  print_str("Hello World!", WHITE_ON_BLACK);
  print_str("HELLO WORLD!", WHITE_ON_BLACK);
  int num = 0;
  for (int i = 0; i < 25; ++i) {
    for (int j = 0; j < 80; ++j) {
      char c[2];
      c[0] = '0' + (num % 10);
      c[1] = '\0';
      print_str(&c, WHITE_ON_BLACK);
      ++num;
    }
  }
  return;
}

