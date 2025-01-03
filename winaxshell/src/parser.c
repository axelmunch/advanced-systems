#include "parser.h"

void parse_command(char *input)
{
    char *args[MAX_ARGS];
    char *token = strtok(input, " \t\n");
    int i = 0;

    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i] = token;
        token = strtok(NULL, " \t\n");
        i++;
    }
    args[i] = NULL;

    if (args[0] == NULL)
    {
        return;
    }

    execute_builtin_command(args);
    execute_external_command(args);
}
