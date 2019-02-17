#ifndef DRIVERS_SCREEN_H
#define DRIVERS_SCREEN_H

#define SCREEN_MAX_ROWS 25
#define SCREEN_MAX_COLS 80
#define SCREEN_WHITE_ON_BLACK 0x0f

/**
 * Prints the given string under the cursor in the terminal
 */
void print_str(const char* c, int attr);

/**
 * Prints the given string under the cursor with color SCREEN_WHITE_ON_BLACK
 */
void kprint(const char* c);

/**
 * Puts the given character at the given position
 */
void screen_put_char(char c, int row, int col, int attr);

/**
 * Sets the cursor at the given position
 */
void screen_set_cursor(int row, int col);

/**
 * Clears the terminal
 */
void clear_terminal();

#endif

