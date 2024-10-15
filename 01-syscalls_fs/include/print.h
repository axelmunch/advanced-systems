#ifndef PRINT_H
#define PRINT_H

#include "constants.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void _print_generic(int fd, const char* format, va_list args);
void print_generic(int fd, const char* format, ...);
void print(const char* format, ...);
void print_error(const char* format, ...);
void set_verbose_mode(bool mode);
bool get_verbose_mode();

#endif // PRINT_H