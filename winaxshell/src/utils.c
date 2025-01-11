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