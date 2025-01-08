#ifndef MAIN_H
#define MAIN_H

#include <pwd.h>
#include "typedef.h"
#include "print.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"

/**
 * @brief Interactive shell mode code execution
 * @return int Exit status
 */
int interactive_mode();

/**
 * @brief Batch mode shell execution
 * @param int argc
 * @param char** argv
 * @return int Exit status
 */
int batch_mode(int argc, char **argv);

/**
 * @brief Print shell prompt
 * @return void
 */
void print_prompt();

#endif // MAIN_H
