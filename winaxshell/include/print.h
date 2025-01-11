#ifndef PRINT_H
#define PRINT_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"

/**
 * @brief Internal print function to a file descriptor
 * @param fd File descriptor
 * @param format Message format
 * @param args Message arguments
 * @return void
 */
void _print_generic(int fd, const char* format, va_list args);

/**
 * @brief Print a message to the file descriptor using internal print generic function
 * @param fd File descriptor
 * @param format Message format
 * @param ... Message arguments
 * @return void
 */
void print_generic(int fd, const char* format, ...);

/**
 * @brief Print a message to the standard output
 * @param format Message format
 * @param ... Message arguments
 * @return void
 */
void print(const char* format, ...);

/**
 * @brief Print an error message to the standard error
 * @param format Message format
 * @param ... Message arguments
 * @return void
 */
void print_error(const char* format, ...);

/**
 * @brief Set the verbose mode
 * @param mode Verbose mode
 * @return void
 */
void set_verbose_mode(bool mode);

/**
 * @brief Get the verbose mode
 * @return bool
 */
bool get_verbose_mode();

#endif // PRINT_H