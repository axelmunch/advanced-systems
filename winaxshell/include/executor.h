#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"
#include "typedef.h"
#include "parser.h"

int execute_single_command(char **args);
int execute_pipe_command(command_node_t *left, command_node_t *right);
int execute_command_tree(command_node_t *node);
void execute_command(char** args);

#endif // COMMAND_H
