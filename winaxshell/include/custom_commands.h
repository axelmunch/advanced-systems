#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "print.h"
#include "constants.h"
#include "custom_ls.h"
#include "custom_cd.h"
#include "alias.h"

/**
 * @brief Test if a command is a custom command
 * @param command Command to test
 * @return bool
 */
bool is_custom_command(char *command);

/**
 * @brief Test if a command is a custom command in the main process
 * @param command Command to test
 * @return bool
 */
bool is_custom_command_main_process(char *command);

/**
 * @brief Execute a custom command
 * @param command Command to execute
 * @param args Command arguments
 * @return void
 */
void execute_custom_command(char *command, char **args);

/**
 * @brief Execute a custom command in the main process (cd)
 * @param command Command to execute
 * @param args Command arguments
 * @return bool Executed successfully
 */
bool execute_custom_command_main_process(char *command, char **args);

#endif // CUSTOM_COMMANDS_H
