#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "constants.h"
#include "utils.h"

/**
 * @brief Create a new command node
 * @return command_node_t* New command node
 */
command_node_t *create_command_node();

/**
 * @brief Parse a command string and create a command tree
 * @param input Command string
 * @return command_tree_t*
 */
command_tree_t* parse_command(char* input);

/**
 * @brief Get the operator type
 * @param operator_str Operator string format
 * @return operator_t enum type
 */
operator_t get_operator_type(const char *operator_str);

/**
 * @brief Free the command tree, including all nodes and arguments
 * @param node Command node
 */
void free_command_tree(command_node_t* node);


#endif // PARSER_H
