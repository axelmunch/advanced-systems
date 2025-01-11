#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "constants.h"
#include "print.h"

/**
 * @brief Procedure checks if variable must be free(check: ptr != NULL)
 * @param void* to_free pointer to an allocated mem
 * @see man 3 free
 * @return void
 */
void free_if_needed(void *to_free);

/**
 * @brief Duplicate the option argument string (optarg)
 * @return void
 */
char *dup_optarg_str();

/**
 * @brief Print help and exit
 * @param char** argv
 * @return void
 */
void show_help(char **argv);

/**
 * @brief Show the binary parameters
 * @param bool verbose_mode
 * @return void
 */
void show_parameters(bool verbose_mode);

/**
 * @brief Check binary requirements
 * @return void
 */
void check_requirements();

/**
 * @brief Parse the binary options
 * @param int argc
 * @param char** argv
 * @return void
 */
void parse_options(int argc, char **argv);

/**
 * @brief Safe open file descriptor
 * @param const char* path
 * @param int flags
 * @param mode_t mode
 */
int safe_open(const char *path, int flags, mode_t mode);

/**
 * @brief Safe close file descriptor
 * @param int fd
 */
void safe_close(int fd);

#endif // UTILS_H