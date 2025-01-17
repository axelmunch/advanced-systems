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
 * @brief Enhanced strtok function that handles quoted strings
 * @param char* String to tokenize
 * @param const char* delimiters
 * @param char** next_token pointer to the next token
 * @return char* token
 */
char *enhanced_strtok(char *str, const char *delim, char **next_token);

/**
 * @brief Add command to history file
 * @param const char* entry command to add to history
 * @return void
 */
void add_history_entry(const char *entry);

#endif // UTILS_H
