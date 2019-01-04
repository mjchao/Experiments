#include "drivers/ports.h"

const static int VIDEO_ADDRESS = 0xb8000;
const static int MAX_COLS = 80;

const static int REG_SCREEN_CTRL = 0x3d4;
const static int REG_SCREEN_DATA = 0x3d5;


static int row_from_position(int position);
static int col_from_position(int position);
static int position_from_row_col(int row, int col);
static int get_cursor_position();
static void set_cursor_position(int row, int col);
static void print_char_at_position(char c, int row, int col);

void print_str(const char* c, int attr) {
  char* curr = c;
  int cursor_position = get_cursor_position();
  int cursor_row = row_from_position(cursor_position);
  int cursor_col = col_from_position(cursor_position);
  while (*curr != '\0') {
    print_char_at_position(*curr, cursor_position, attr);  
    ++cursor_col;
    if (cursor_col == 80) {
      cursor_col = 0;
      cursor_row += 1;
    }
    cursor_position = position_from_row_col(cursor_row, cursor_col);
    set_cursor_position(cursor_row, cursor_col);
    ++curr; 
  }
}

static void print_char_at_position(char c, int position, int attr) {
  unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
  vidmem[position] = c;
  vidmem[position + 1] = attr;
}

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

static void set_cursor_position(int row, int col) {
  int position = row * MAX_COLS + col;

  // set high byte
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (position >> 8));

  // set low byte
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (position & 0xff));
}

static int position_from_row_col(int row, int col) {
  return 2 * (row * MAX_COLS + col);
}

static int row_from_position(int position) {
  return position / 2 / MAX_COLS;
}

static int col_from_position(int position) {
  return position / 2 % MAX_COLS;
}

