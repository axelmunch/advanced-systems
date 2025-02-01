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

    if (current->args[index] != NULL)
        free_if_needed(current->args[index]); // Free any existing argument

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

char *handle_env_assignment(char *str, const char *delim, char **next_token)
{
    static char *last_allocated = NULL;

    if (last_allocated != NULL)
    {
        free_if_needed(last_allocated);
        last_allocated = NULL;
    }

    char *equals = strchr(str, EQUAL_SIGN);
    if (!equals || equals <= str || equals >= str + strcspn(str, delim))
        return NULL;

    char *value_start = equals + 1;
    while (*value_start && isspace(*value_start))
        value_start++;

    if (*value_start != DOUBLE_QUOTES && *value_start != SINGLE_QUOTE)
        return NULL;

    char *quote_end = handle_quoted_string(value_start, *value_start, next_token);

    if (quote_end)
    {
        last_allocated = quote_end;
        return str;
    }

    return NULL;
}

char *expand_env_vars(const char *str)
{
    if (!str || !strchr(str, DOLLAR_SIGN))
        return strdup(str);

    size_t total_size = 0;
    const char *read_pos = str;

    while (*read_pos)
    {
        if (*read_pos == DOLLAR_SIGN && *(read_pos + 1))
        {
            char var_name[256] = {0};
            var_name[0] = DOLLAR_SIGN;
            int i = 1;
            while (isalnum(read_pos[i]) || read_pos[i] == '_')
            {
                var_name[i] = read_pos[i];
                i++;
            }

            char *value = get_env_var(var_name);
            if (value)
            {
                total_size += strlen(value);
                free_if_needed(value);
            }
            read_pos += i;
        }
        else
        {
            total_size++;
            read_pos++;
        }
    }

    char *result = malloc(total_size + 1);
    if (!result)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to allocate memory");
        return NULL;
    }

    read_pos = str;
    char *write_pos = result;

    while (*read_pos)
    {
        if (*read_pos == DOLLAR_SIGN && *(read_pos + 1))
        {
            char var_name[256] = {0};
            var_name[0] = DOLLAR_SIGN;
            int i = 1;
            while (isalnum(read_pos[i]) || read_pos[i] == '_')
            {
                var_name[i] = read_pos[i];
                i++;
            }

            char *value = get_env_var(var_name);
            if (value)
            {
                size_t len = strlen(value);
                memcpy(write_pos, value, len);
                write_pos += len;
                free_if_needed(value);
            }
            read_pos += i;
        }
        else
        {
            *write_pos++ = *read_pos++;
        }
    }
    *write_pos = NULL_CHAR;
    return result;
}

char *handle_quoted_string(char *str, char quote, char **next_token)
{
    str++;
    char *end = str;
    char *result = NULL;

    while (*end && *end != quote)
        end++;

    if (*end == quote)
    {
        *end = NULL_CHAR;
        *next_token = end + 1;

        if (quote == DOUBLE_QUOTES || quote == SINGLE_QUOTE)
            result = expand_env_vars(str);
        else
            result = strdup(str);

        return result;  // enhanced_strtok will free this
    }
    return NULL;
}

char *enhanced_strtok(char *str, const char *delim, char **next_token)
{
    static char *last_allocated = NULL;

    if (last_allocated != NULL)
    {
        free_if_needed(last_allocated); // Free any previously result
        last_allocated = NULL;
    }

    if (str == NULL)
        str = *next_token;

    str += strspn(str, delim);
    if (*str == NULL_CHAR)
        return NULL;

    char *token = str;
    char *env_result = handle_env_assignment(str, delim, next_token);
    if (env_result)
        return env_result;

    if (*str == DOUBLE_QUOTES || *str == SINGLE_QUOTE)
    {
        char *result = handle_quoted_string(str, *str, next_token);
        last_allocated = result;
        return result;
    }

    str += strcspn(str, delim);

    if (*str)
        *str++ = NULL_CHAR;

    *next_token = str;
    return token;
}
