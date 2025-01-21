#include "parser.h"

command_node_t *create_command_node()
{
    command_node_t *node = malloc(sizeof(command_node_t));
    if (node == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to allocate memory for command node");
        return NULL;
    }

    node->args = malloc(MAX_ARGS * sizeof(char *));
    if (node->args == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to allocate memory for command node arguments");
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

command_tree_t *create_command_tree()
{
    command_tree_t *tree = malloc(sizeof(command_tree_t));
    if (tree == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to allocate memory for command tree");
        return NULL;
    }

    tree->root = create_command_node();
    if (tree->root == NULL)
    {
        free(tree);
        return NULL;
    }

    return tree;
}

command_node_t *handle_operator(command_tree_t *tree, operator_t op)
{
    command_node_t *new_node = create_command_node();
    if (new_node == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to create new command node");
        return NULL;
    }

    new_node->left = tree->root;
    new_node->op_type = op;
    new_node->right = create_command_node();
    if (new_node->right == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to create right command node");
        free_command_node(new_node);
        return NULL;
    }

    return new_node;
}

int handle_argument(command_node_t *current, const char *token, size_t index)
{
    if (index >= MAX_ARGS - 1)
    {
        errno = E2BIG;
        print_error("[ERROR] Too many arguments");
        current->args[MAX_ARGS - 1] = NULL;
        return EXIT_FAILURE;
    }

    char *stored_value = NULL;

    if (token[0] == DOLLAR_SIGN)
    {
        char *env_value = get_env_var(token);
        if (env_value != NULL)
            stored_value = env_value;
        else
            stored_value = strdup("");
    }
    else
    {
        stored_value = strdup(token);
    }

    if (stored_value == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to duplicate argument string");
        return EXIT_FAILURE;
    }

    current->args[index] = stored_value;

    return EXIT_SUCCESS;
}

command_tree_t *parse_command(const char *input)
{
    if (input == NULL)
        return NULL;

    char *input_copy = strdup(input);
    if (input_copy == NULL)
    {
        print_error("[ERROR] Failed to duplicate input string");
        return NULL;
    }

    command_tree_t *tree = create_command_tree();
    if (tree == NULL)
    {
        free_if_needed(input_copy);
        print_error("[ERROR] Failed to create command tree");
        return NULL;
    }

    command_node_t *current = tree->root;
    size_t arg_index = 0;
    char *next_token = NULL;
    char *token = enhanced_strtok(input_copy, CMD_DELIMITER, &next_token);

    while (token != NULL)
    {
        if (strchr(token, EQUAL_SIGN) != NULL)
        {
            if (set_env_var(token) != 0)
            {
                print_error("[ERROR] Failed to set environment variable");
                free_command_node(tree->root);
                free_if_needed(tree);
                free_if_needed(input_copy);
                return NULL;
            }
            token = enhanced_strtok(NULL, CMD_DELIMITER, &next_token);
            continue;
        }

        operator_t op = get_operator_type(token);
        if (op != OP_NONE)
        {
            command_node_t *new_node = handle_operator(tree, op);
            if (new_node == NULL)
            {
                print_error("[ERROR] Failed to handle operator");
                free_command_node(tree->root);
                free_if_needed(tree);
                free_if_needed(input_copy);
                return NULL;
            }
            tree->root = new_node;
            current = new_node->right;
            arg_index = 0;
        }
        else if (handle_argument(current, token, arg_index++) == EXIT_FAILURE)
        {
            free_command_node(tree->root);
            free_if_needed(tree);
            free_if_needed(input_copy);
            return NULL;
        }
        token = enhanced_strtok(NULL, CMD_DELIMITER, &next_token);
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
    else if (strcmp(operator_str, ">") == 0)
        return OP_REDIR_OUT;
    else if (strcmp(operator_str, "<") == 0)
        return OP_REDIR_IN;
    else if (strcmp(operator_str, ">>") == 0)
        return OP_APPEND;
    else if (strcmp(operator_str, "<<") == 0)
        return OP_HEREDOC;
    else
        return OP_NONE;
}
