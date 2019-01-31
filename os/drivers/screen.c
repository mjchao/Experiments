#include "drivers/screen.h"
#include "drivers/ports.h"

const static int VIDEO_ADDRESS = 0xb8000;

const static int REG_SCREEN_CTRL = 0x3d4;
const static int REG_SCREEN_DATA = 0x3d5;

static int row_from_position(int position);
static int col_from_position(int position);
static int position_from_row_col(int row, int col);
static int get_cursor_position();
static void set_cursor_position(int row, int col);
static void print_char_at_position(char c, int row, int col);
static void scroll_one_row();


void kprint(const char* c) {
  print_str(c, SCREEN_WHITE_ON_BLACK);
}


void print_str(const char* c, int attr) {
  const char* curr = c;
  int cursor_position = get_cursor_position();
  int cursor_row = row_from_position(cursor_position);
  int cursor_col = col_from_position(cursor_position);
  while (*curr != '\0') {
    if (*curr == '\n') {
      cursor_row += 1;
      cursor_col = 0;
    } else {
      print_char_at_position(*curr, cursor_position, attr);  
      ++cursor_col;
    }
    if (cursor_col == SCREEN_MAX_COLS) {
      cursor_col = 0;
      cursor_row += 1;
    }
    if (cursor_row == SCREEN_MAX_ROWS) {
      scroll_one_row();
      cursor_row = SCREEN_MAX_ROWS - 1;
      cursor_col = 0;
    }
    cursor_position = position_from_row_col(cursor_row, cursor_col);
    set_cursor_position(cursor_row, cursor_col);
    ++curr; 
  }
}

/**
 * Clears the entire terminal
 */
void clear_terminal() {
  for (int row = 0; row < SCREEN_MAX_ROWS; ++row) {
    for (int col = 0; col < SCREEN_MAX_COLS; ++col) {
      int position = position_from_row_col(row, col);
      print_char_at_position(' ', position, SCREEN_WHITE_ON_BLACK);
    }
  }
  set_cursor_position(0, 0);
}

/**
 * Prints a character at the given position in video memory
 */
static void print_char_at_position(char c, int position, int attr) {
  unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
  vidmem[position] = c;
  vidmem[position + 1] = attr;
}

/**
 * @return gets the location at which the cursor is in video memory (if you
 * print a character at this position, it would go on the cursor).
 */
static int get_cursor_position() {
  // request high byte of cursor position
  port_byte_out(REG_SCREEN_CTRL, 14);

  // shift byte by 8 so that it becomes the high byte
  int position = port_byte_in(REG_SCREEN_DATA);
  position = position << 8;

  // request low byte of cursor position
  port_byte_out(REG_SCREEN_CTRL, 15);
  position += port_byte_in(REG_SCREEN_DATA);
  
  // multiply by 2 because each cell consists of 1 byte control data and
  // 1 byte for character
  return position * 2;
}

/**
 * Sets the cursor to the specified row and column of video memory
 */
static void set_cursor_position(int row, int col) {
  int position = row * SCREEN_MAX_COLS + col;

  // set high byte
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (position >> 8));

  // set low byte
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (position & 0xff));
}

/**
 * @return position in video memory for the given (row, col) on the terminal
 */
static int position_from_row_col(int row, int col) {
  return 2 * (row * SCREEN_MAX_COLS + col);
}

/**
 * @return row of the terminal corresponding to the given position
 */
static int row_from_position(int position) {
  return position / 2 / SCREEN_MAX_COLS;
}

/**
 * @return column of the terminal corresponding to the given position
 */
static int col_from_position(int position) {
  return position / 2 % SCREEN_MAX_COLS;
}

/**
 * Scrolls down one row. Row 0 of the terminal is lost, row 1 becomes row 0,
 * row 2 becomes row 1, ..., and the last row is cleared.
 */
void scroll_one_row() {
  unsigned char* vidmem = (unsigned char*) VIDEO_ADDRESS;
  for (int row = 1; row < SCREEN_MAX_ROWS; ++row) {
    for (int col = 0; col < SCREEN_MAX_COLS; ++col) {
      int src_pos = position_from_row_col(row, col);
      int dest_pos = position_from_row_col(row - 1, col);
      vidmem[dest_pos] = vidmem[src_pos];
      vidmem[dest_pos + 1] = vidmem[src_pos + 1];
    }
  }
  for (int col = 0; col < SCREEN_MAX_COLS; ++col) {
    int clear_pos = position_from_row_col(SCREEN_MAX_ROWS - 1, col);
    vidmem[clear_pos] = ' ';
    vidmem[clear_pos + 1] = SCREEN_WHITE_ON_BLACK;
  }
}

