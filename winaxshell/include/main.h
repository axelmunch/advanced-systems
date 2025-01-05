#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <pwd.h>
#include "typedef.h"
#include "print.h"
#include "parser.h"
#include "command.h"

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
 * @brief Interactive shell mode code execution
 * @return void
 */
void interactive_mode();

/**
 * @brief Batch mode shell execution
 * @param int argc
 * @param char** argv
 * @return void
 */
void batch_mode(int argc, char **argv);

/**
 * @brief Print shell prompt
 * @return void
 */
void print_prompt();

#endif // MAIN_H
