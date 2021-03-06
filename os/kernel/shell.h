#ifndef KERNEL_SHELL_H
#define KERNEL_SHELL_H

/**
 * Initializes the shell.
 */
void shell_init();

/**
 * Clears the shell.
 */
void shell_clear();

/**
 * Prints a message from the kernel under the cursor.
 */
void shell_kprint(const char* msg);

/**
 * Prints a message under the cursor with color attributes
 */
void shell_print_with_attr(const char* msg, int attr);

/**
 * Prints a message under the cursor.
 */
void shell_print(const char* msg);
#endif

