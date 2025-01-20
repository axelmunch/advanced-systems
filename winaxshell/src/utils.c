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

char *enhanced_strtok(char *str, const char *delim, char **next_token)
{
    char *token;
    if (str == NULL)
        str = *next_token;

    str += strspn(str, delim);
    if (*str == '\0')
        return NULL;

    token = str;

    // For environment variable assignment
    char *equals = strchr(str, EQUAL_SIGN);
    if (equals != NULL && equals > str && equals < str + strcspn(str, delim))
    {
        char *value_start = equals + 1;
        while (*value_start && isspace(*value_start))
            value_start++;

        if (*value_start == DOUBLE_QUOTES || *value_start == SINGLE_QUOTE)
        {
            char quote = *value_start;
            char *quote_end = value_start + 1;

            while (*quote_end && *quote_end != quote)
                quote_end++;

            if (*quote_end == quote)
            {
                *next_token = quote_end + 1;
                return token;
            }
        }
    }

    if (*str == DOUBLE_QUOTES || *str == SINGLE_QUOTE)
    {
        char quote = *str;
        str++;
        token = str;

        while (*str && *str != quote)
            str++;

        if (*str == quote)
        {
            *str = '\0';
            *next_token = str + 1;
            return token;
        }
    }

    str += strcspn(str, delim);
    if (*str)
        *str++ = '\0';

    *next_token = str;
    return token;
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

    char *home = getenv("HOME");
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

int set_env_var(const char *token)
{
    char *equal_sign = strchr(token, '=');
    if (!equal_sign)
        return -1;

    size_t name_len = equal_sign - token;
    char *env_name = strndup(token, name_len);

    if (!env_name)
        return -1;

    const char *value = equal_sign + 1;
    char *processed_value = NULL;
    size_t value_len;

    while (isspace(*value))
        value++;

    if (*value == DOUBLE_QUOTES || *value == SINGLE_QUOTE)
    {
        char quote_char = *value;
        value++;
        const char *end_quote = strchr(value, quote_char);

        if (end_quote)
        {
            value_len = end_quote - value;
            processed_value = strndup(value, value_len);
        }
        else
        {
            processed_value = strdup(value);
        }
    }
    else
    {
        processed_value = strdup(value);
    }

    if (!processed_value)
    {
        free_if_needed(env_name);
        return -1;
    }

    int ret = setenv(env_name, processed_value, 1);
    free_if_needed(env_name);
    free_if_needed(processed_value);

    return ret;
}
