#include "parser.h"

command_node_t *create_command_node()
{
    command_node_t *node = malloc(sizeof(command_node_t));
    if (node == NULL)
    {
        return NULL;
    }
    node->args = malloc(MAX_ARGS * sizeof(char *));
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

operator_t get_operator_type(const char *operator_str)
{
    if (operator_str == NULL)
        return OP_NONE;

    if (strcmp(operator_str, "|") == 0)
        return OP_PIPE;
    else if (strcmp(operator_str, ";") == 0)
        return OP_SEQ;
    else if (strcmp(operator_str, "&&") == 0)
        return OP_AND;
    else if (strcmp(operator_str, "||") == 0)
        return OP_OR;
    else
        return OP_NONE;
}

void free_command_tree(command_node_t *node)
{
    if (node == NULL)
        return;

    free_command_tree(node->left);
    free_command_tree(node->right);

    free(node->args);
    free(node);
}

command_tree_t *parse_command(const char *input)
{
    command_tree_t *tree = malloc(sizeof(command_tree_t));
    if (tree == NULL)
        return NULL;

    char *token = strtok(strdup(input), CMD_DELIMITER);
    if (token == NULL)
    {
        free(tree);
        return NULL;
    }

    command_node_t *current = create_command_node();
    tree->root = current;

    int arg_index = 0;

    while (token != NULL)
    {
        operator_t op = get_operator_type(token);
        if (op != OP_NONE) // operator found
        {
            current->op_type = op;
            current->right = create_command_node();
            if (current->right == NULL)
            {
                free_command_tree(tree->root);
                free(tree);
                return NULL;
            }
            current = current->right;
            arg_index = 0;
        }
        else
        {
            current->args[arg_index] = strdup(token);
            if (arg_index >= MAX_ARGS - 1)
            {
                current->args[arg_index] = NULL;
                break;
            }
        }
        token = strtok(NULL, CMD_DELIMITER);
    }

    current->args[arg_index] = NULL;
    return tree;
}