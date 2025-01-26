#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
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
 * @brief Free a command node
 * @param node Command node to free
 * @return void
 */
void free_command_node(command_node_t* node);

/**
 * @brief Safe open file descriptor with error handling
 * @param const char* path pathname of the file to open
 * @param int flags flags to open the file with
 * @param mode_t mode file mode to open the file with
 */
int safe_open(const char *path, int flags, mode_t mode);

/**
 * @brief Safe close file descriptor with error handling
 * @param int fd: file descriptor to close
 */
void safe_close(int fd);

/**
 * @brief Check if a string is all whitespace
 * @param const char* str string to check
 * @return int 1 if all whitespace, 0 otherwise
 */
int is_all_space(const char *str);

/**
 * @brief Add command to history file
 * @param const char* entry command to add to history
 * @return void
 */
void add_history_entry(const char *entry);

/**
 * @brief Get the environment variable value
 * @param const char* token name of the environment variable
 * @return char* value of the environment variable
 */
char *get_env_var(const char *token);

/**
 * @brief Set the environment variable value
 * @param const char* token name of the environment variable
 * @return int 0 if successful, -1 otherwise
 */
int set_env_var(const char *token);

#endif // UTILS_H
