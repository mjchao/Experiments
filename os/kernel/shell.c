#include "kernel/shell.h"
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include <stdbool.h>


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
        kprint(keycode_str[keycode]);
      } else {
        kprint(shift_keycode_str[keycode]);
      }
    } else {
      kprint(shift_keycode_str[keycode]);
    }
  } else {
    if (is_alpha) {
      if (is_caps_lock) {
        kprint(shift_keycode_str[keycode]);
      } else {
        kprint(keycode_str[keycode]);
      }
    } else {
      kprint(keycode_str[keycode]);
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
      kprint(keycode_str[keycode]);
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


void init_shell() {
  kprint("Initializing shell...\n");

  isr_install();
  asm volatile("sti");
  
  init_keyboard();

  keyboard_on_key_down = on_key_down;
  keyboard_on_key_up = on_key_up;
}
