#include "kernel/util.h"

void int_to_ascii(int n, char str[]) {
  int is_negative = 0;
  if (n < 0) {
    is_negative = 1;
    n = -n;
  }

  int i = 0;
  do {
    str[i++] = n % 10 + '0';
    n /= 10;
  } while (n > 0);
  
  if (is_negative) {
    str[i++] = '-';
  }
  str[i] = '\0';
  reverse(str);
}

void reverse(char s[]) {
  int c, i, j;
  for (i = 0, j = strlen(s) - 1; i < j; ++i, --j) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

int strlen(char s[]) {
  int i = 0;
  while (s[i] != '\0') {
    ++i;
  }
  return i;
}

int min(int a, int b) {
  return a < b ? a : b;
}
