#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H

#include "cpu/types.h"

/**
 * Maps each key to a unique integer code.
 */
typedef u8 KeyCode_t;
enum {
  K_ERROR,  // K_ERROR indicates an error occurred. You should never get K_ERROR
  K_ESC,
  K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0, K_DASH, K_EQ, K_BACKSPACE,
  K_TAB, K_Q, K_W, K_E, K_R, K_T, K_Y, K_U, K_I, K_O, K_P, K_LBRACKET, K_RBRACKET,
  K_ENTER,
  K_CTRL,
  K_A, K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L, K_SEMICOLON, K_APOSTROPHE, K_LEFTQUOTE,
  K_LSHIFT,
  K_BACKSLASH,
  K_Z, K_X, K_C, K_V, K_B, K_N, K_M, K_COMMA, K_PERIOD, K_SLASH, K_RSHIFT,
  K_ASTERISK,
  K_ALT, KSPACE,
  K_CAPSLOCK,
  K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10,
  K_NUMLOCK,
  K_SCROLLLOCK,
  K_KEYPAD7, K_KEYPAD8, K_KEYPAD9,
  K_MINUS,
  K_KEYPAD4, K_KEYPAD5, K_KEYPAD6, K_PLUS,
  K_KEYPAD1, K_KEYPAD2, K_KEYPAD3,
  K_KEYPAD0, K_KEYPADPERIOD
};

/**
 * Maps modifiers to a unique value. 
 */
typedef u8 KeyMod_t;
enum {
  KMOD_CTRL    = 1 << 0, // ctrl key
  KMOD_ALT     = 1 << 2, // alt key 
  KMOD_SHIFT   = 1 << 1, // shift key
  KMOD_EXTEND  = 1 << 3, // extend key (e.g. extending keypad-7 gives "home")
};

/**
 * An event handler for when a keyboard event occurs.
 *
 * @param keycode the keycode for the key that was pressed
 * @param modifiers any modifiers (e.g. shift, ctrl, alt). If the keycode
 *    is for a modifier key, the modifier will represent after the key has been
 *    pressed/released. For example, if the user presses CTRL, the keycode will
 *    be CTRL and the modifier will contain KMOD_CTRL. If the user releases
 *    CTRL, the keycode will be CTRL and the modifier will no longer contain
 *    KMOD_CTRL.
 */
typedef void (*keyevent_handler_t)(KeyCode_t keycode, KeyMod_t modifiers); 

/**
 * Callback when a key is pressed down.
 */
extern keyevent_handler_t* on_key_down;

/**
 * Callback when a key is released.
 */
extern keyevent_handler_t* on_key_up;

/**
 * Initializes the keyboard driver.
 */
void init_keyboard();

#endif
