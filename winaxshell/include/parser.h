#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/**
 * @brief Parse the command line
 * @param input Command line inputs
 * @return char** Command arguments as array
 */
char** parse_command(char *input);

#endif // PARSER_H
