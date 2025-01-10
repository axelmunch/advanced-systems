#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "print.h"
#include "typedef.h"
#include "parser.h"

/**
 * @brief Execute a single command
 * @param args Command arguments
 * @return int Exit status
 */
int execute_single_command(char **args);

/**
 * @brief Execute a pipe command
 * @param left Left command
 * @param right Right command
 * @return int Exit status
 */
int execute_pipe_command(command_node_t *left, command_node_t *right);

/**
 * @brief Execute a background command
 * @param node Command node
 * @return int Exit status
 */
int execute_background_command(command_node_t *node);

/**
 * @brief Execute a command tree. This function recursively executes commands represented in a tree structure.
 * It handles different types of commands (e.g., single commands, pipes) by traversing the tree and executing each node appropriately.
 * @param node Command node
 * @return int Exit status
 */
int execute_command_tree(command_node_t *node);

/**
 * @brief Execute a command. This function parses the input string and creates a command tree, then executes the command tree.
 * @param input User input string from the shell
 * @param command_tree Command tree to execute
 * @return void
 */
void execute_command(char* input, command_tree_t *command_tree);

#endif // COMMAND_H
