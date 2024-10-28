#include "reverse.h"
#include <sys/stat.h>

void reverse_file_data(int src_fd, int dst_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    off_t file_size, offset;

    // Get file size
    file_size = lseek(src_fd, 0, SEEK_END);
    if (file_size == -1) {
        print_error("[!] Error getting file size");
        close_file(src_fd);
        close_file(dst_fd);
        exit(EXIT_FAILURE);
    }

    // Bufferized reverse
    for (offset = file_size; offset > 0; offset -= BUFFER_SIZE) {
        ssize_t to_read = (offset >= BUFFER_SIZE) ? BUFFER_SIZE : offset;
        if (lseek(src_fd, offset - to_read, SEEK_SET) == -1) {
            print_error("[!] Error seeking in source file");
            close_file(src_fd);
            close_file(dst_fd);
            exit(EXIT_FAILURE);
        }

        bytes_read = read(src_fd, buffer, to_read);
        if (bytes_read == -1) {
            print_error("[!] Error reading from source file");
            close_file(src_fd);
            close_file(dst_fd);
            exit(EXIT_FAILURE);
        }

        // Reverse
        for (int i = 0; i < bytes_read / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[bytes_read - i - 1];
            buffer[bytes_read - i - 1] = tmp;
        }

        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            print_error("[!] Error writing to destination file");
            close_file(src_fd);
            close_file(dst_fd);
            exit(EXIT_FAILURE);
        }
        print("[DEBUG] Wrote %ld bytes to destination\n", bytes_written);
    }
}

void reverse(const char *src, const char *dst) {
    print("[+] Reversing %s to %s\n", src, dst);

    print("[+] Opening %s file for reading...\n", src);
    int src_fd = open_file(src, O_RDONLY, 0);
    print("[+] Source file opened successfully\n");

    print("[+] Opening %s file for writing...\n", dst);
    int dst_fd = open_file(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    print("[+] Destination file opened successfully\n");

    print("[+] Reversing data from %s to %s...\n", src, dst);
    reverse_file_data(src_fd, dst_fd);

    print("[+] File copy completed successfully\n");

    close_file(src_fd);
    close_file(dst_fd);
}
