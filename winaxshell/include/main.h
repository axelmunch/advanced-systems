#ifndef MAIN_H
#define MAIN_H

#include <pwd.h>
#include "typedef.h"
#include "print.h"
#include "parser.h"
#include "executor.h"
#include "helper.h"
#include "alias.h"

/**
 * @brief Interactive shell mode code execution
 * @return int Exit status
 */
int interactive_mode(void);

/**
 * @brief Batch mode shell execution
 * @param int argc
 * @param char** argv
 * @return int Exit status
 */
int batch_mode(int argc, char **argv);

#endif // MAIN_H
