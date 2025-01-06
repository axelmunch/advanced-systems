#include "executor.h"

int execute_single_command(char **args)
{
    if (args == NULL || args[0] == NULL)
        return EXIT_FAILURE;

    if (strcmp(args[0], "exit") == 0)
    {
        print_generic(STDOUT_FILENO, GREEN_COLOR "Bye! Thanks for using WinAxShell!\n" RESET_COLOR);
        exit(EXIT_SUCCESS);
    }
}