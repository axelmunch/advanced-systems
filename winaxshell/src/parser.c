#include "parser.h"

char** parse_command(char *input)
{
    char **args = malloc(MAX_ARGS * sizeof(char *));
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
        free(args);
        return NULL;
    }

    return args;
}
