#include "drivers/keyboard.h"
#include "drivers/ports.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include "cpu/types.h"
#include "kernel/util.h"
#include <stdbool.h>


keyevent_handler_t* on_key_down = NULL;
keyevent_handler_t* on_key_up = NULL;


char* scancode_text[] = {
                                                                              // Scancode (base 10)
  "ERROR",                                                                    // 0
  "ESC",                                                                      // 1
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE",    // 2 - 14
  "TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]",          // 15 - 27
  "ENTER",                                                                    // 28
  "CTRL",                                                                     // 29
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


static bool is_num_lock = false;
static bool is_caps_lock = false;
static bool is_scroll_lock = false;
static bool is_ctrl = false;
static bool is_alt = false;
static bool is_shift = false;
static bool is_extend = false;
const static int NUM_RECOGNIZED_CODES = sizeof(scancode_text) / sizeof(char*);

static u8 build_modifiers() {
  int mod = 0;
  if (is_ctrl) {
    mod |= KMOD_CTRL;
  }
  if (is_alt) {
    mod |= KMOD_ALT;
  }
  if ((is_shift && !is_caps_lock) || (!is_shift && is_caps_lock)) {
    mod |= KMOD_SHIFT;
  }
  if (is_extend) {
    mod |= KMOD_EXTEND;
  }
  return mod;
}

static void handle_key_down(u8 code) {
  switch (code) {
  
  case K_CTRL:
    is_ctrl = true;
    break;

  case K_ALT:
    is_alt = true;
    break;

  case K_LSHIFT:
  case K_RSHIFT:
    is_shift = true;
    break;

  case K_NUMLOCK:
    is_num_lock = !is_num_lock;
    break;

  case K_CAPSLOCK:
    is_caps_lock = !is_caps_lock;
    break;

  case K_SCROLLLOCK:
    is_scroll_lock = !is_scroll_lock;
    break;

  case K_ESC:
    // ignore
    break;

  default:
    break;
  }

  if (on_key_down != NULL) {
    (*on_key_down)(code, build_modifiers());
  }
}

static void handle_key_up(u8 code) {
  switch (code) {
  
  case K_CTRL:
    is_ctrl = false;
    break;

  case K_ALT:
    is_alt = false;
    break;

  case K_LSHIFT:
  case K_RSHIFT:
    is_shift = false;
    break;

  default:
    // ignore
    break;
  }

  if (on_key_up != NULL) {
    (*on_key_up)(code, build_modifiers());
  }
}

static void keyboard_callback(registers_t regs) {
  u8 scancode = port_byte_in(0x60);

  if (scancode <= 128) {
    if (scancode < NUM_RECOGNIZED_CODES) {
      int key_index = scancode;
      handle_key_down(key_index);
    } else {
      kprint("Unknown key down\n");
    }
  } else if (scancode <= 128 + NUM_RECOGNIZED_CODES) {
    int key_index = scancode - 128;
    handle_key_up(key_index);
  } else if (scancode == 224) {
    is_extend = !is_extend;
  }
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}

