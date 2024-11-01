#ifndef REDIRECT_H
#define REDIRECT_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"

void open_temp();
int show_msg(char *message);
void process_child(char *program_name);
void process_parent();
void _redirect_stdout(char *program_name);
void _redirect_stderr(char *program_name);
void redirect(char *program_name);

#endif // REDIRECT_H
