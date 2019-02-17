#include "drivers/keyboard.h"
#include "drivers/ports.h"
#include "drivers/screen.h"
#include "cpu/isr.h"
#include "cpu/types.h"
#include "kernel/util.h"
#include <stdbool.h>


keyevent_handler_t keyboard_on_key_down = NULL;
keyevent_handler_t keyboard_on_key_up = NULL;


static bool is_num_lock = false;
static bool is_caps_lock = false;
static bool is_scroll_lock = false;
static bool is_ctrl = false;
static bool is_alt = false;
static bool is_shift = false;
static bool is_extend = false;
const static int NUM_RECOGNIZED_CODES = K_NUMBER_OF_KEYS;

static u8 build_modifiers() {
  int mod = 0;
  if (is_ctrl) {
    mod |= KMOD_CTRL;
  }
  if (is_alt) {
    mod |= KMOD_ALT;
  }
  if (is_shift) {
    mod |= KMOD_SHIFT;
  }
  if (is_extend) {
    mod |= KMOD_EXTEND;
  }
  if (is_num_lock) {
    mod |= KMOD_NUMLOCK;
  }
  if (is_caps_lock) {
    mod |= KMOD_CAPSLOCK;
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

  if (keyboard_on_key_down != NULL) {
    keyboard_on_key_down(code, build_modifiers());
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

  if (keyboard_on_key_up != NULL) {
    keyboard_on_key_up(code, build_modifiers());
  }
}

static void keyboard_callback(registers_t regs) {
  u8 scancode = port_byte_in(0x60);

  if (scancode == 224) {
    is_extend = true;
    return;
  }

  if (scancode <= 128) {
    if (scancode < NUM_RECOGNIZED_CODES) {
      int key_index = scancode;
      handle_key_down(key_index);
    } else {
      // ignore unknown key down
    }
  } else if (scancode <= 128 + NUM_RECOGNIZED_CODES) {
    int key_index = scancode - 128;
    handle_key_up(key_index);
  }
  is_extend = false;
}

void init_keyboard() {
  register_interrupt_handler(IRQ1, keyboard_callback);
}

