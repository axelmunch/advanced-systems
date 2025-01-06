#include "command.h"

void execute_external_command(char **args)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        print_error("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            print_error("Command execution failed");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute_command(char **args)
{
    if (args[0] == NULL)
    {
        return;
    }

    if (strcmp(args[0], "exit") == 0) // implémenter la commande exit, à améliorer
    {
        print_generic(STDOUT_FILENO, GREEN_COLOR "Bye! Thanks for using WinAxShell!\n" RESET_COLOR);
        exit(EXIT_SUCCESS);
    }
    else
    {
        execute_external_command(args);
    }
}
