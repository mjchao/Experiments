#include "kernel/shell.h"
#include "kernel/util.h"
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include <stdbool.h>

#define SHELL_HISTORY_MAX_ROWS 25
#define SHELL_INPUT_MAX_ROWS 3
#define SHELL_MAX_COLS SCREEN_MAX_COLS

typedef struct {
  int attr;
  char c;
} ScreenChar;

struct Shell {
  // TODO allocate from heap.
  ScreenChar history[SHELL_HISTORY_MAX_ROWS][SHELL_MAX_COLS];
  ScreenChar input[SHELL_INPUT_MAX_ROWS][SHELL_MAX_COLS];
  int history_start_row;
  int history_end_row;
  int history_end_col;

  int input_end_row;
  int input_end_col;

  int cursor_row;
  int cursor_col;
} shell;

// -------------- Text Rendering Logic --------------- //

// Renders user input, and then renders history.
static void shell_render() {
  for (int row = 0; row < SCREEN_MAX_ROWS; ++row) {
    for (int col = 0; col < SCREEN_MAX_COLS; ++col) {
      screen_put_char('\0', row, col, SCREEN_WHITE_ON_BLACK);
    }
  }

  // figure out how many rows of the screen we'll be taking up
  int required_screen_rows = min(SCREEN_MAX_ROWS,
      (shell.input_end_row + 1) + (shell.history_end_row + 1));

  // TODO render user input - something is horribly messed up
  int screen_input_start_row = required_screen_rows - 1 - shell.input_end_row;
  /*for (int row = 0; row <= shell.input_end_row; ++row) {
    int screen_row = screen_input_start_row + row;
    int end_col = (row == shell.input_end_row) ? shell.input_end_col :
      SHELL_MAX_COLS;
    for (int col = 0; col < end_col; ++col) {
      screen_put_char(shell.input[row][col].c, row, col,
          shell.input[row][col].attr);
    }
  }*/

  // render shell history
  for (int screen_row = 0; screen_row < screen_input_start_row; ++screen_row) {
    int history_row = (shell.history_start_row + screen_row) %
        SHELL_HISTORY_MAX_ROWS;
    int end_col = (screen_row == screen_input_start_row - 1) ?
        shell.history_end_col : SHELL_MAX_COLS;
    for (int col = 0; col < end_col; ++col) {
      screen_put_char(shell.history[history_row][col].c, screen_row, col,
          shell.history[history_row][col].attr);
    }
  }
}

static void shell_clear_history() {
  for (int row = 0; row < SHELL_HISTORY_MAX_ROWS; ++row) {
    for (int col = 0; col < SHELL_MAX_COLS; ++col) {
      shell.history[row][col].c = 0;
      shell.history[row][col].attr = SCREEN_WHITE_ON_BLACK;
    }
  }
  shell.history_start_row = 0;
  shell.history_end_row = 0;
  shell.history_end_col = 0;
}

static void shell_add_history_newline() {
  shell.history_end_col = 0;
  shell.history_end_row += 1;

  // wrapped around and time to start throwing away old history
  if (shell.history_end_row == SHELL_HISTORY_MAX_ROWS) {
    shell.history_end_row = 0;
  } 
  if (shell.history_end_row == shell.history_start_row) {
    shell.history_start_row += 1;
  }
}

static void shell_add_to_history(char c, int attr) {
  shell.history[shell.history_end_row][shell.history_end_col].c = c;
  shell.history[shell.history_end_row][shell.history_end_col].attr = attr;
  shell.history_end_col += 1;

  // end of line
  if (shell.history_end_col == SHELL_MAX_COLS) {
    shell_add_history_newline();
  }
}

static void shell_clear_input() {
  for (int row = 0; row < SHELL_INPUT_MAX_ROWS; ++row) {
    for (int col = 0; col < SHELL_MAX_COLS; ++col) {
      shell.input[row][col].c = 0;
      shell.input[row][col].attr = SCREEN_WHITE_ON_BLACK;
    }
  }
  shell.input_end_row = 0;
  shell.input_end_col = 0;
}

static void shell_add_to_input(char c, int attr) {
  if (shell.input_end_row == SHELL_INPUT_MAX_ROWS - 1 &&
      shell.input_end_col == SHELL_MAX_COLS) {
    // refuse to process more user input
    return;
  }

  shell.input[shell.input_end_row][shell.input_end_col].c = c;
  shell.input[shell.input_end_row][shell.input_end_col].attr = attr;
  shell.input_end_col += 1;
  if (shell.input_end_col == SHELL_MAX_COLS) {
    shell.input_end_row += 1;
    shell.input_end_col = 0;
  }
}

static void shell_print_char_with_attr(char c, int attr);
void shell_print_input_with_attr(const char* input, int attr) {
  for (const char* c = input; *c != '\0'; ++c) {
    // XXX something horribly wrong with rendering input - probably in
    // bootsect. Just print as non-input for now.
    shell_print_char_with_attr(*c, attr);
  }
  shell_render();
}

void shell_print_input(const char* input) {
  shell_print_input_with_attr(input, SCREEN_WHITE_ON_BLACK);
}

static void shell_print_tab(int attr) {
  for (int i = 0; i < 4; ++i) {
    shell_print_char_with_attr(' ', attr);
  }
}

static void shell_print_char_with_attr(char c, int attr) {
  if (c == '\n') {
    shell_add_history_newline();
  } else if (c == '\t') {
    shell_print_tab(attr);

  // normal characters that only take up one space
  } else {
    shell_add_to_history(c, attr);
  }
}

void shell_print_with_attr(const char* msg, int attr) {
  for (const char* c = msg; *c != '\0'; ++c) {
    shell_print_char_with_attr(*c, attr);
  }
  shell_render();
}

void shell_kprint(const char* msg) {
  shell_print_with_attr(msg, SCREEN_WHITE_ON_BLACK);
}

void shell_print(const char* msg) {
  shell_print_with_attr(msg, SCREEN_WHITE_ON_BLACK);
}

// -------------------------------------------------------------- //





// ------------- Key Listener Logic ------------------------- //
/**
 * Maps each keycode to a string that can be printed to the shell
 */
static char* keycode_str[] = {
                                                                              // Scancode (base 10)
  "ERROR",                                                                    // 0
  "ESC",                                                                      // 1
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE",    // 2 - 14
  "TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]",          // 15 - 27
  "\n",                                                                       // 28
  "CTRL",                                                                     // 29
  "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`",                 // 30 - 41
  "LSHIFT",                                                                   // 42
  "\\",                                                                       // 43
  "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "RSHIFT",                 // 44 - 54
  "*",                                                                        // 55
  "ALT", " ",                                                                 // 56 - 57
  "CAPSLOCK",                                                                 // 58
  "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",                // 59 - 68
  "NumLock",                                                                  // 69
  "ScrollLock",                                                               // 70
  "7", "8", "9",                                                              // 71 -73
  "-",                                                                        // 74
  "4", "5", "6", "+",                                                         // 75 - 78
  "1", "2", "3",                                                              // 79 - 81
  "0", ".",                                                                   // 82 - 83
  "RESERVED",
};

static char* shift_keycode_str[] = {
                                                                              // Scancode (base 10)
  "ERROR",                                                                    // 0
  "ESC",                                                                      // 1
  "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "BACKSPACE",    // 2 - 14
  "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}",          // 15 - 27
  "\n",                                                                       // 28
  "CTRL",                                                                     // 29
  "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"", "~",                // 30 - 41
  "LSHIFT",                                                                   // 42
  "|",                                                                        // 43
  "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "RSHIFT",                 // 44 - 54
  "*",                                                                        // 55
  "ALT", " ",                                                                 // 56 - 57
  "CAPSLOCK",                                                                 // 58
  "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",                // 59 - 68
  "NumLock",                                                                  // 69
  "ScrollLock",                                                               // 70
  "7", "8", "9",                                                              // 71 -73
  "-",                                                                        // 74
  "4", "5", "6", "+",                                                         // 75 - 78
  "1", "2", "3",                                                              // 79 - 81
  "0", ".",                                                                   // 82 - 83
  "RESERVED", // maps K_NUMBER_OF_KEYS just in case it gets triggered
};

static void print_char(KeyCode_t keycode, KeyMod_t modifiers) {
  bool is_shift = (modifiers & KMOD_SHIFT); 
  bool is_caps_lock = (modifiers & KMOD_CAPSLOCK);
  bool is_alpha = (K_Q <= keycode && keycode <= K_P) ||
      (K_A <= keycode && keycode <= K_L) ||
      (K_Z <= keycode && keycode <= K_M);

  if (is_shift) {
    if (is_alpha) {
      if (is_caps_lock) {
        shell_print_input(keycode_str[keycode]);
      } else {
        shell_print_input(shift_keycode_str[keycode]);
      }
    } else {
      shell_print_input(shift_keycode_str[keycode]);
    }
  } else {
    if (is_alpha) {
      if (is_caps_lock) {
        shell_print_input(shift_keycode_str[keycode]);
      } else {
        shell_print_input(keycode_str[keycode]);
      }
    } else {
      shell_print_input(keycode_str[keycode]);
    }
  }
}

static void handle_keypad_down(KeyCode_t keycode, KeyMod_t modifiers) {
  bool is_num_lock = (modifiers & KMOD_NUMLOCK);
  bool is_extend = (modifiers & KMOD_EXTEND);
  if (is_extend) {
    // TODO handle keypad + extend modifier
  } else {
    if (is_num_lock) {
      shell_print(keycode_str[keycode]);
    } else {
      // TODO HOME, UP, DOWN, etc.
    }
  }
}

static void on_key_down(KeyCode_t keycode, KeyMod_t modifiers) {
  switch (keycode) {

  // these keys don't ever affect what's displayed in the shell
  case K_ERROR:
  case K_ESC:
  case K_CTRL:
  case K_LSHIFT:
  case K_RSHIFT:
  case K_ALT:
  case K_CAPSLOCK:
  case K_NUMLOCK:
  case K_SCROLLLOCK:
  case K_NUMBER_OF_KEYS:
    break;

  case K_KEYPAD7:
  case K_KEYPAD8:
  case K_KEYPAD9:
  case K_KEYPAD4:
  case K_KEYPAD5:
  case K_KEYPAD6:
  case K_KEYPAD1:
  case K_KEYPAD2:
  case K_KEYPAD3:
  case K_KEYPAD0:
  case K_KEYPADPERIOD:
    handle_keypad_down(keycode, modifiers);
    break;

  case K_BACKSPACE:
    // TODO: backspace
    break;

  case K_TAB:
    // TODO: tab
    break;

  default:
    print_char(keycode, modifiers);
    break;
  }
}

static void on_key_up(KeyCode_t keycode, KeyMod_t modifiers) {

}

// ---------------------------------------------------------------- //

void shell_init() {
  shell_clear_history();
  shell_clear_input();
  shell_kprint("Initializing shell...\n");

  isr_install();
  asm volatile("sti");

  init_keyboard();
  keyboard_on_key_down = on_key_down;
  keyboard_on_key_up = on_key_up;
}
