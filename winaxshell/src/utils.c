#include "utils.h"

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

void free_command_node(command_node_t *node)
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
    free_command_node(node->left);
    free_command_node(node->right);
    free_if_needed(node);
}

int safe_open(const char *path, int flags, mode_t mode)
{
    int fd = open(path, flags, mode);
    if (fd < 0)
    {
        print_error("[ERROR] open() failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void safe_close(int fd)
{
    int ret = close(fd);
    if (ret < 0)
    {
        print_error("[ERROR] close() failed");
        exit(EXIT_FAILURE);
    }
}

int is_all_space(const char *str)
{
    while (*str)
    {
        if (!isspace(*str))
            return 0;
        str++;
    }
    return 1;
}

void add_history_entry(const char *entry)
{
    if (entry == NULL || is_all_space(entry))
        return;

    char *home = getenv(HOME_ENV_VAR);
    if (home == NULL)
    {
        print_error("[ERROR] HOME environment variable not set");
        return;
    }

    char *history_file = malloc(strlen(home) + strlen(HISTORY_FILE) + 2);
    if (history_file == NULL)
    {
        errno = ENOMEM;
        print_error("[ERROR] Failed to allocate memory for history file path");
        return;
    }

    sprintf(history_file, "%s/%s", home, HISTORY_FILE);
    int history_fd = safe_open(history_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

    print_generic(history_fd, "%s\n", entry);
    safe_close(history_fd);
    free_if_needed(history_file);
}

char *get_env_var(const char *token)
{
    if (token[0] == DOLLAR_SIGN)
    {
        const char *env_name = token + 1;
        const char *env_value = getenv(env_name);
        if (env_value != NULL)
        {
            return strdup(env_value);
        }

        return strdup("");
    }

    return NULL;
}

int get_var_values(const char *token, char *name, char *value)
{
    char *equal_sign = strchr(token, EQUAL_SIGN);
    if (!equal_sign)
        return -1;

    size_t name_len = equal_sign - token;
    char *processed_name = strndup(token, name_len);

    if (!processed_name)
        return -1;

    const char *current_value = equal_sign + 1;
    char *processed_value = NULL;
    size_t value_len;

    while (isspace(*current_value))
        current_value++;

    if (*current_value == DOUBLE_QUOTES || *current_value == SINGLE_QUOTE)
    {
        char quote_char = *current_value;
        current_value++;
        const char *end_quote = strchr(current_value, quote_char);

        if (end_quote)
        {
            value_len = end_quote - current_value;
            processed_value = strndup(current_value, value_len);
        }
        else
        {
            processed_value = strdup(current_value);
        }
    }
    else
    {
        processed_value = strdup(current_value);
    }

    if (!processed_value)
    {
        free_if_needed(processed_name);
        return -1;
    }

    strcpy(name, processed_name);
    strcpy(value, processed_value);

    free_if_needed(processed_name);
    free_if_needed(processed_value);

    return 0;
}

int set_env_var(const char *token)
{
    // Get values from get_var_values
    char name[MAX_INPUT_LENGTH];
    char value[MAX_INPUT_LENGTH];
    int ret = get_var_values(token, name, value);
    if (ret < 0)
        return ret;

    ret = setenv(name, value, 1);

    return ret;
}

const char *get_operator_str(operator_t op_type)
{
    switch (op_type)
    {
    case OP_NONE:
        return "NONE";
    case OP_PIPE:
        return "PIPE";
    case OP_SEQ:
        return "SEQ";
    case OP_AND:
        return "AND";
    case OP_OR:
        return "OR";
    case OP_BG:
        return "BG";
    case OP_REDIR_OUT:
        return "REDIR_OUT";
    case OP_REDIR_IN:
        return "REDIR_IN";
    case OP_APPEND:
        return "APPEND";
    case OP_HEREDOC:
        return "HEREDOC";
    default:
        return "UNKNOWN";
    }
}

void print_command_tree(command_node_t *node, int depth)
{
    // int show_debug = 1;

    // if (!show_debug)
    //     return;

    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        print_generic(STDERR_FILENO, "  ");

    print_generic(STDERR_FILENO, "Node: op_type=%s, args=[", get_operator_str(node->op_type));

    if (node->args)
    {
        for (int i = 0; node->args[i] != NULL; i++)
            print_generic(STDERR_FILENO, "%s%s", i > 0 ? ", " : "", node->args[i] ? node->args[i] : "NULL");
    }
    print_generic(STDERR_FILENO, "]\n");

    if (node->left)
    {
        for (int i = 0; i < depth; i++)
            print_generic(STDERR_FILENO, "  ");

        print_generic(STDERR_FILENO, "Left child:\n");
        print_command_tree(node->left, depth + 1);
    }

    if (node->right)
    {
        for (int i = 0; i < depth; i++)
            print_generic(STDERR_FILENO, "  ");

        print_generic(STDERR_FILENO, "Right child:\n");
        print_command_tree(node->right, depth + 1);
    }
}
