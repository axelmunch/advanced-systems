#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "print.h"
#include "pipe.h"

int exec_ps(char *arg, ...);
int exec_grep(char *arg, ...);
int exec_echo(char *arg, ...);
void pipe_process();

#endif // PIPE_H