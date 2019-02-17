#include "drivers/screen.h"
#include "drivers/ports.h"

const static int VIDEO_ADDRESS = 0xb8000;

const static int REG_SCREEN_CTRL = 0x3d4;
const static int REG_SCREEN_DATA = 0x3d5;

/**
 * @return position in video memory for the given (row, col) on the terminal
 */
static int position_from_row_col(int row, int col) {
  return 2 * (row * SCREEN_MAX_COLS + col);
}

/**
 * Prints a character at the given position in video memory
 */
void screen_put_char(char c, int row, int col, int attr) {
  int position = position_from_row_col(row, col);
  unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
  vidmem[position] = c;
  vidmem[position + 1] = attr;
}

/**
 * Sets the cursor to the specified row and column of video memory
 */
void screen_set_cursor(int row, int col) {
  int position = row * SCREEN_MAX_COLS + col;

  // set high byte
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (position >> 8));

  // set low byte
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (position & 0xff));
}

