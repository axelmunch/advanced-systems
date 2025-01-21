#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "typedef.h"
#include "constants.h"
#include "utils.h"

/**
 * @brief Create a new command node
 * @return command_node_t* New command node
 */
command_node_t *create_command_node();

/**
 * @brief Create a new command tree
 * @return command_tree_t* New command tree
 */
command_tree_t *create_command_tree();

/**
 * @brief Handle an operator
 * @param tree Command tree to add operator to
 * @param op Operator type to add
 * @return command_node_t* New command node
 */
command_node_t *handle_operator(command_tree_t *tree, operator_t op);

/**
 * @brief Handle an argument
 * @param node Command node to add argument to
 * @param arg Argument string to add
 * @param index Argument index in the command
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int handle_argument(command_node_t *node, const char *arg, size_t index);

/**
 * @brief Parse a command string and create a command tree
 * @param input Command string to parse
 * @return command_tree_t*
 */
command_tree_t* parse_command(const char* input);

/**
 * @brief Get the operator type
 * @param operator_str Operator string format
 * @return operator_t enum type
 */
operator_t get_operator_type(const char *operator_str);

#endif // PARSER_H
