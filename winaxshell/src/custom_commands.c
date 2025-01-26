#include "custom_commands.h"

static const char *custom_commands_list[] = {
    "ls",
    "echo",
    NULL};

bool is_custom_command(char *command)
{
    for (int i = 0; custom_commands_list[i] != NULL; i++)
    {
        if (strcmp(command, custom_commands_list[i]) == 0)
        {
            return true;
        }
    }

    return false;
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
    else if (strcmp(command, "echo") == 0)
    {
        for (int i = 1; i < argc; i++)
        {
            print_generic(STDOUT_FILENO, "%s ", args[i]);
        }
        print_generic(STDOUT_FILENO, "\n");
    }

    exit(status);
}
