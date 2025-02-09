#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <getopt.h>
#include "constants.h"
#include "print.h"

/**
 * @brief Get the requested command in command mode
 * @return char* command
 */
char *get_requested_command();

/**
 * @brief Duplicate the option argument string (optarg)
 * @return char* duplicated string
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
 * @param bool command_mode
 * @return void
 */
void show_parameters(bool verbose_mode, bool command_mode);

/**
 * @brief Check binary requirements
 * @param int argc
 * @param char** argv
 * @return void
 */
void check_requirements(int argc, char **argv);

/**
 * @brief Parse the binary options
 * @param int argc
 * @param char** argv
 * @param bool* command_mode
 * @param bool* no_execute
 * @return void
 */
void parse_options(int argc, char **argv, bool *command_mode, bool *no_execute);

/**
 * @brief Print shell prompt
 * @return void
 */
void print_prompt();

/**
 * @brief Print the iconic predator shake
 * @return void
 */
void predator_shake();

#endif // HELPER_H
