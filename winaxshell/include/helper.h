#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <getopt.h>
#include "constants.h"
#include "print.h"

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
 * @param int argc
 * @param char** argv
 * @return void
 */
void check_requirements(int argc, char **argv);

/**
 * @brief Parse the binary options
 * @param int argc
 * @param char** argv
 * @return void
 */
void parse_options(int argc, char **argv);

/**
 * @brief Print shell prompt
 * @return void
 */
void print_prompt();

#endif // HELPER_H
