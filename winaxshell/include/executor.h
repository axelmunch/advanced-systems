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

/**
 * @brief Execute a single command
 * @param args Command arguments
 * @return int
 */
int execute_single_command(char **args);

/**
 * @brief Execute a pipe command
 * @param left Left command
 * @param right Right command
 * @return int
 */
int execute_pipe_command(command_node_t *left, command_node_t *right);

/**
 * @brief Execute a command tree. This function recursively executes commands represented in a tree structure.
 * It handles different types of commands (e.g., single commands, pipes) by traversing the tree and executing each node appropriately.
 * @param node Command node
 * @return int
 */
int execute_command_tree(command_node_t *node);

/**
 * @brief Execute a command. This function executes a command given its arguments.
 * It is a wrapper around execute_single_command and other execution functions.
 * @param args Command arguments
 */
void execute_command(char **args);

#endif // COMMAND_H
