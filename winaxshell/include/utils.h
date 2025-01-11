#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <fcntl.h>
#include "typedef.h"
#include "print.h"

/**
 * @brief Procedure checks if variable must be free(check: ptr != NULL)
 * @param void* to_free pointer to an allocated mem
 * @see man 3 free
 * @return void
 */
void free_if_needed(void *to_free);

/**
 * @brief Safe open file descriptor with error handling
 * @param const char* path
 * @param int flags
 * @param mode_t mode
 */
int safe_open(const char *path, int flags, mode_t mode);

/**
 * @brief Safe close file descriptor with error handling
 * @param int fd
 */
void safe_close(int fd);

/**
 * @brief Free a command node
 * @param node Command node
 * @return void
 */
void free_command_node(command_node_t* node);

#endif // UTILS_H