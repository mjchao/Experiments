#define VIDEO_ADDRESS 0xb8000
#define WHITE_ON_BLACK 0x0f
#define MAX_COLS 80


void print_char_at(char c, int row, int col) {
  unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
  int position = 2 * (row * MAX_COLS + col);
  vidmem[position] = c;
  vidmem[position + 1] = WHITE_ON_BLACK;
}
