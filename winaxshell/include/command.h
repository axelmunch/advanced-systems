#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "print.h"

/**
 * @brief Execute an external command
 * @param args Command arguments
 * @return void
 */
void execute_external_command(char **args);

/**
 * @brief Execute a builtin command
 * @param args Command arguments
 * @return void
 */
void execute_builtin_command(char **args);


#endif // COMMAND_H
