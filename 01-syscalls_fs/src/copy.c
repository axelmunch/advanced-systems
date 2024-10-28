#include "copy.h"

int open_file(const char *filename, int flags, mode_t mode)
{
    int fd = open(filename, flags, mode);
    if (fd == -1)
    {
        print_error("[!] Failed to open file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void copy_file_data(int src_fd, int dst_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read)
        {
            print_error("[!] Error writing to destination file");
            close_file(src_fd);
            close_file(dst_fd);
            exit(EXIT_FAILURE);
        }
        print("[DEBUG] Wrote %ld bytes to destination\n", bytes_written);
    }

    if (bytes_read == -1)
    {
        print_error("[!] Error reading from source file");
        exit(EXIT_FAILURE);
    }
}

void close_file(int fd)
{
    if (close(fd) == -1)
    {
        print_error("[!] Failed to close file");
        exit(EXIT_FAILURE);
    }
}

void copy(const char *src, const char *dst)
{
    print("[+] Copying %s to %s\n", src, dst);

    print("[+] Opening %s file for reading...\n", src);
    int src_fd = open_file(src, O_RDONLY, 0);
    print("[+] Source file opened successfully\n");

    print("[+] Opening %s file for writing...\n", dst);
    int dst_fd = open_file(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    print("[+] Destination file opened successfully\n");

    print("[+] Copying data from %s to %s...\n", src, dst);
    copy_file_data(src_fd, dst_fd);

    print("[+] File copy completed successfully\n");

    close_file(src_fd);
    close_file(dst_fd);
}
