#include "print.h"

bool is_verbose_mode = false;

void _print_generic(int fd, const char *format, va_list args)
{
    char buffer[BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    write(fd, buffer, strlen(buffer));
}

void print_generic(int fd, const char *format, ...)
{
    va_list args;

    va_start(args, format);

    _print_generic(fd, format, args);

    va_end(args);
}

void print(const char *format, ...)
{
    if (!get_verbose_mode())
    {
        return;
    }

    va_list args;

    va_start(args, format);

    _print_generic(STDOUT, format, args);

    va_end(args);
}

void print_error(const char *format, ...)
{
    va_list args;

    va_start(args, format);

    _print_generic(STDERR, format, args);
    if (strlen(format) > 0)
    {
        print_generic(STDERR, ": ");
    }
    print_generic(STDERR, "%s\n", strerror(errno));

    va_end(args);
}

void set_verbose_mode(bool mode)
{
    is_verbose_mode = mode;
}

bool get_verbose_mode()
{
    return is_verbose_mode;
}
