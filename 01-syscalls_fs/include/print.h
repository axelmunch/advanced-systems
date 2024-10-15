#include "constants.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void _print_generic(int fd, const char* format, va_list args);
void print_generic(int fd, const char* format, ...);
void print(const char* format, ...);
void print_error(const char* format, ...);
