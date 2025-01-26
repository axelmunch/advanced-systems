#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "print.h"
#include "custom_ls.h"

bool is_custom_command(char *command);
void execute_custom_command(char *command, char **args);

#endif // CUSTOM_COMMANDS_H
