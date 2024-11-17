#ifndef REDIRECT_H
#define REDIRECT_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"

void open_temp(int redirect_fd);
void exec_program(char *program_name);
int show_msg(char *message);
void process_child(char *program_name, int redirect_fd);
void process_parent();
void redirect(char *program_name, int redirect_fd);

#endif // REDIRECT_H
