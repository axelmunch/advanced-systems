#include "copy.h"

void copy(const char *src, const char *dst) 
{
    print("[+] Copying %s to %s\n", src, dst);
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    print("[+] Opening %s file for reading...\n", src);
    src_fd = open(src, O_RDONLY);
    if (src_fd == -1) 
    {
        perror("[!] Failed to open source file");
        exit(EXIT_FAILURE);
    }
    print("[+] Source file opened successfully\n");

    print("[+] Opening %s file for writing...\n", dst);
    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1) 
    {
        perror("[!] Failed to open destination file");
        close(src_fd);
        exit(EXIT_FAILURE);
    }
    print("[+] Destination file opened successfully\n");

    print("[+] Copying data from %s to %s...\n", src, dst);
    size_t buffer_size = sizeof(buffer);
    while ((bytes_read = read(src_fd, buffer, buffer_size)) > 0)
    {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) 
        {
            perror("[!] Error writing to destination file");
            close(src_fd);
            close(dst_fd);
            exit(EXIT_FAILURE);
        }
        print("[DEBUG] Wrote %ld bytes to destination\n", bytes_written);
    }

    if (bytes_read == -1)
        perror("[!] Error reading from source file");

    print("[+] File copy completed successfully\n");

    close(src_fd);
    close(dst_fd);
}
