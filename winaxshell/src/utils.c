#include "utils.h"

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
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