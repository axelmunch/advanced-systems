#include "../include/print.h"

void _print_generic(int fd, const char* format, va_list args)
{
    char buffer[BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    write(fd, buffer, strlen(buffer));
}

void print_generic(int fd, const char* format, ...)
{
    va_list args;

    va_start(args, format);

    _print_generic(fd, format, args);

    va_end(args);
}

void print(const char* format, ...)
{
    va_list args;

    va_start(args, format);

    _print_generic(STDOUT, format, args);

    va_end(args);
}

void print_error(const char* format, ...)
{
    va_list args;

    va_start(args, format);

    _print_generic(STDERR, format, args);

    va_end(args);
}
