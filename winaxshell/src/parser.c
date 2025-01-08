#include "parser.h"

command_node_t *create_command_node()
{
    command_node_t *node = malloc(sizeof(command_node_t));

    if (node == NULL)
        return NULL;

    node->args = malloc(MAX_ARGS * sizeof(char *));
    if (node->args == NULL)
    {
        free_if_needed(node);
        return NULL;
    }

    for (int i = 0; i < MAX_ARGS; i++)
    {
        node->args[i] = NULL;
    }
    node->op_type = OP_NONE;
    node->left = NULL;
    node->right = NULL;

    return node;
}

command_tree_t *parse_command(char *input)
{
    if (input == NULL)
        return NULL;

    command_tree_t *tree = malloc(sizeof(command_tree_t));
    if (tree == NULL)
        return NULL;

    char *input_copy = strdup(input);
    if (input_copy == NULL)
    {
        free_if_needed(tree);
        return NULL;
    }

    char *token = strtok(input_copy, CMD_DELIMITER);
    if (token == NULL)
    {
        free_if_needed(tree);
        return NULL;
    }

    command_node_t *current = create_command_node();
    if (current == NULL)
    {
        free_if_needed(input_copy);
        free_if_needed(tree);
        return NULL;
    }

    tree->root = current;
    int arg_index = 0;

    while (token != NULL)
    {
        operator_t op = get_operator_type(token);
        if (op != OP_NONE)
        {
            command_node_t *new_node = create_command_node();
            if (new_node == NULL)
            {
                free_command_tree(tree->root);
                free_if_needed(tree);
                free_if_needed(input_copy);
                return NULL;
            }

            new_node->left = tree->root;
            new_node->op_type = op;
            new_node->right = create_command_node();
            if (new_node->right == NULL)
            {
                free_command_tree(new_node);
                free_command_tree(tree->root);
                free_if_needed(tree);
                free_if_needed(input_copy);
                return NULL;
            }

            tree->root = new_node;
            current = new_node->right;
            arg_index = 0;
        }
        else
        {
            if (arg_index >= MAX_ARGS - 1)
            {
                current->args[MAX_ARGS - 1] = NULL;
                break;
            }
            current->args[arg_index] = strdup(token);
            if (current->args[arg_index] == NULL)
            {
                free_command_tree(tree->root);
                free_if_needed(tree);
                free_if_needed(input_copy);
                return NULL;
            }
            arg_index++;
        }
        token = strtok(NULL, CMD_DELIMITER);
    }
    current->args[arg_index] = NULL;
    free_if_needed(input_copy);
    return tree;
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
    else if (strcmp(operator_str, "&") == 0)
        return OP_BG;
    else
        return OP_NONE;
}

void free_command_tree(command_node_t *node)
{
    if (node == NULL)
        return;

    if (node->args != NULL)
    {
        for (int i = 0; i < MAX_ARGS && node->args[i] != NULL; i++)
        {
            free_if_needed(node->args[i]);
            node->args[i] = NULL;
        }
        free_if_needed(node->args);
        node->args = NULL;
    }
    
    free_command_tree(node->left);
    free_command_tree(node->right);
    free_if_needed(node);
}
