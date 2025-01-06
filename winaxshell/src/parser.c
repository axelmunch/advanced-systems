#include "parser.h"

command_node_t *create_command_node()
{
    command_node_t* node = malloc(sizeof(command_node_t));
    if (node == NULL)
    {
        return NULL;
    }
    node->args = malloc(MAX_ARGS * sizeof(char*));
    if (node->args == NULL)
    {
        free(node);
        return NULL;
    }

    node->op_type = OP_NONE;
    node->left = NULL;
    node->right = NULL;

    return node;
}