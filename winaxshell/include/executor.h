#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "print.h"
#include "typedef.h"
#include "parser.h"
#include "custom_commands.h"

/**
 * @brief Execute a single command
 * @param args Command arguments
 * @return int Exit status
 */
int execute_single_command(char **args);

/**
 * @brief Execute a pipe command
 * @param node Command node root of the tree
 * @return int Exit status
 */
int execute_pipe_command(command_node_t *node);

/**
 * @brief Execute a background command
 * @param node Command node
 * @return int Exit status
 */
int execute_background_command(command_node_t *node);

/**
 * @brief Execute a redirection command
 * @param left Left command
 * @param right Right command
 * @param redirect_type Redirector operator type
 * @return int Exit status
 */
int execute_redirection_command(command_node_t *left, command_node_t *right, operator_t redirect_type);

/**
 * @brief Execute a command or a custom command
 * @param command Command to execute
 * @param args Command arguments
 * @return int Exit status
 */
int custom_exec(char *command, char **args);

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
 * @return int Exit status
 */
int execute_command(char* input, command_tree_t *command_tree);

#endif // COMMAND_H
