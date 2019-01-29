#include "drivers/keyboard.h"
#include "drivers/ports.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include "kernel/util.h"


char* keyboard_vals[] = {
                                                                              // Scancode (base 10)
  "ERROR",                                                                    // 0
  "ESC",                                                                      // 1
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE",    // 2 - 14
  "TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]",          // 15 - 27
  "ENTER",                                                                    // 28
  "LCTRL",                                                                    // 29
  "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`",                 // 30 - 41
  "LSHIFT",                                                                   // 42
  "\\",                                                                       // 43
  "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "RSHIFT",                 // 44 - 54
  "*",                                                                        // 55
  "ALT", "SPACE",                                                             // 56 - 57
  "CAPSLOCK",                                                                 // 58
  "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",                // 59 - 68
  "NumLock",                                                                  // 69
  "ScrollLock",                                                               // 70
  "Keypad-7/Home", "Keypad-8/Up", "Keypad-9/PgUp",                            // 71 -73
  "Keypad--",                                                                 // 74
  "Keypad-4/Left", "Keypad-5", "Keypad-6/Right", "Keypad-+",                  // 75 - 78
  "Keypad-1/End", "Keypad-2/Down", "Keypad-3/PgDn",                           // 79 - 81
  "Keypad-0/Ins", "Keypad-./Del"                                              // 82 - 83
};

void print_letter(u8 scancode) {
  int num_recognized_codes = sizeof(keyboard_vals) / sizeof(char*);
  if (scancode < num_recognized_codes) {
    kprint(keyboard_vals[scancode]);
  } else {
    if (scancode <= 0x7f) {
        kprint("Unknown key down");
    } else if (scancode <= 0x39 + 0x80) {
        kprint("key up ");
    } else {
      kprint("Unknown key up");
    }
  }
}

static void keyboard_callback(registers_t regs) {
  u8 scancode = port_byte_in(0x60);
  print_letter(scancode);
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}

