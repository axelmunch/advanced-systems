#ifndef BUFFERED_IO_H
#define BUFFERED_IO_H

#include <stdbool.h>
#include "constants.h"
#include "typedef.h"

FICHIER *my_open(const char *filename, const char *mode);
int my_close(FICHIER *f);
int my_getc(FICHIER *f);
int my_putc(int c, FICHIER *f);

#endif // BUFFERED_IO_H
