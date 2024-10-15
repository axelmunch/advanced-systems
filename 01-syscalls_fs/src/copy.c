#include "../include/copy.h"

void copy(const char *src, const char *dst)
{
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    src_fd = open(src, O_RDONLY);
    if (src_fd == -1)
    {
        perror("Failed to open src file");
        exit(EXIT_FAILURE);
    }

    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1)
    {
        perror("Failed to open dst file");
        close(src_fd);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            perror("Failed to write to dst file");
            close(src_fd);
            close(dst_fd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1)
        perror("Failed to read from src file");

    close(src_fd);
    close(dst_fd);
}
