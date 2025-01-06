#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "constants.h"

command_node_t *create_command_node();
operator_t get_operator_type(const char *operator_str);
void free_command_tree(command_node_t* node);
command_tree_t* parse_command(const char* input);

#endif // PARSER_H
