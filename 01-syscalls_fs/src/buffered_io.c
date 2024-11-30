#include "buffered_io.h"
#include <stdlib.h>
#include <fcntl.h>
#include "print.h"

FICHIER *my_open(const char *filename, const char *mode)
{
    FICHIER *f = malloc(sizeof(FICHIER));
    if (f == NULL)
    {
        print_error("[ERROR] Failed to allocate memory for FICHIER");
        return NULL;
    }

    // Open file with correct mode
    if (strcmp(mode, "r") == 0)
    {
        f->fd = open(filename, O_RDONLY);
        f->write_mode = false;
    }
    else if (strcmp(mode, "w") == 0)
    {
        f->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        f->write_mode = true;
    }
    else
    {
        print_error("[ERROR] Invalid mode");
        free(f);
        return NULL;
    }

    if (f->fd == -1)
    {
        print_error("[ERROR] Failed to open file");
        free(f);
        return NULL;
    }

    f->index = 0;
    f->buffer_size_taken = 0;

    return f;
}

int my_close(FICHIER *f)
{
    // Write rest of buffer if in write mode
    if (f->write_mode && f->index > 0)
    {
        if (write(f->fd, f->buffer, f->index) == -1)
        {
            print_error("[ERROR] Error writing to file (rest of buffer)");
            free(f);
            return -1;
        }
    }

    if (close(f->fd) == -1)
    {
        print_error("[ERROR] Failed to close file");
        free(f);
        return -1;
    }

    free(f);
    return 0;
}

int my_getc(FICHIER *f)
{
    if (f->index == f->buffer_size_taken)
    {
        f->buffer_size_taken = read(f->fd, f->buffer, BUFFER_SIZE);
        if (f->buffer_size_taken == -1)
        {
            print_error("[ERROR] Error reading from file");
            return -1;
        }
        f->index = 0;
    }

    if (f->buffer_size_taken == 0)
    {
        return EOF;
    }

    f->index++;

    return f->buffer[f->index - 1];
}

int my_putc(int c, FICHIER *f)
{
    if (!f->write_mode)
    {
        print_error("[ERROR] File not opened in write mode");
        return -1;
    }

    if (f->index == BUFFER_SIZE)
    {
        if (write(f->fd, f->buffer, BUFFER_SIZE) == -1)
        {
            print_error("[ERROR] Error writing to file");
            return -1;
        }
        f->index = 0;
    }

    f->buffer[f->index] = c;
    f->index++;

    return c;
}
