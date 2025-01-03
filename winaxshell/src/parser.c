#include "parser.h"

void parse_command(char *input)
{
    char *args[MAX_ARGS];
    char *token = strtok(input, CMD_DELIMITER);
    int arg_index = 0;

    while (token != NULL && arg_index < MAX_ARGS - 1)
    {
        args[arg_index] = token;
        token = strtok(NULL, CMD_DELIMITER);
        arg_index++;
    }
    args[arg_index] = NULL;

    if (args[0] == NULL)
    {
        return;
    }

    execute_command(args);
}
