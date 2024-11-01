#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "print.h"
#include "pipe.h"

int exec_ps(char *arg, ...);
int exec_grep(char *arg, ...);
int exec_echo(char *arg, ...);
void handle_ps(int pipe_fd[2]);
void handle_grep(int pipe_fd[2]);
void handle_parent(pid_t grep_pid);
void pipe_process();

#endif // PIPE_H
