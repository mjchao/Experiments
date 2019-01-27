#define WHITE_ON_BLACK 0x0f

/**
 * Prints the given string under the cursor in the terminal
 */
void print_str(const char* c, int attr);

/**
 * Prints the given string under the cursor with WHITE_ON_BLACK
 */
void kprint(const char* c);

/**
 * Clears the terminal
 */
void clear_terminal();
