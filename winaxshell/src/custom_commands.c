#include "custom_commands.h"

bool is_custom_command(char *command)
{
    return strcmp(command, "ls") == 0;
}

void execute_custom_command(char *command, char **args)
{
    if (!is_custom_command(command))
    {
        errno = EINVAL;
        print_error("[ERROR] Command is not a custom command");
        exit(EXIT_FAILURE);
        return;
    }

    int argc = 0;
    while (args[argc] != NULL)
    {
        argc++;
    }

    int status = EXIT_SUCCESS;

    // ls
    if (strcmp(command, "ls") == 0)
    {
        char *param = ".";
        if (argc > 2)
        {
            errno = EINVAL;
            print_error("[ERROR] Too many arguments for ls");
            exit(EXIT_FAILURE);
            return;
        }

        if (argc == 2)
        {
            param = args[1];
        }
        status = ls(param);
    }

    exit(status);
}
