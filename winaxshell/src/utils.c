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
    if (*str == NULL_CHAR)
        return NULL;

    if (*str == DOUBLE_QUOTES)
    {
        str++;
        token = str;

        while (*str && *str != DOUBLE_QUOTES)
            str++;

        if (*str == DOUBLE_QUOTES)
        {
            *str = NULL_CHAR;
            str++;
        }
    }
    else if (*str == SINGLE_QUOTE)
    {
        str++;
        token = str;

        while (*str && *str != SINGLE_QUOTE)
            str++;

        if (*str == SINGLE_QUOTE)
        {
            *str = NULL_CHAR;
            str++;
        }
    }
    else
    {
        token = str;
        str += strcspn(str, delim);
        if (*str)
        {
            *str = NULL_CHAR;
            str++;
        }
    }
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
    if (equal_sign != NULL)
    {
        char *env_name = strndup(token, equal_sign - token);
        if (env_name == NULL)
        {
            print_error("[ERROR] Failed to allocate memory for env name");
            return -1;
        }

        const char *env_value = equal_sign + 1;
        char *processed_value = NULL;

        while (isspace(*env_value)) env_value++;

        if (*env_value == DOUBLE_QUOTES || *env_value == SINGLE_QUOTE)
        {
            char quote = *env_value;
            env_value++;

            char *end_quote = strchr(env_value, quote);

            if (end_quote != NULL)
                processed_value = strndup(env_value, end_quote - env_value);
            else
                processed_value = strdup(env_value);
        }
        else
        {
            processed_value = strdup(env_value);
        }

        if (processed_value == NULL)
        {
            free_if_needed(env_name);
            print_error("[ERROR] Failed to allocate memory for env value");
            return -1;
        }

        int ret = setenv(env_name, processed_value, 1);
        free_if_needed(env_name);
        free_if_needed(processed_value);

        if (ret < 0)
        {
            print_error("[ERROR] Failed to set environment variable");
            return -1;
        }
        return 0;
    }
    return -1;
}
