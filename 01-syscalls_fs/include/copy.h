#ifndef COPY_H
#define COPY_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "print.h"
#include "constants.h"

void copy(const char *src, const char *dst);

#endif // COPY_H