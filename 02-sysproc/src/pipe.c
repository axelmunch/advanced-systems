#include "pipe.h"

int exec_ps(char *arg, ...)
{
    return execlp("/bin/ps", "ps", arg, NULL);
}

int exec_grep(char* arg, ...)
{
    return execlp("/bin/grep", "grep", arg, NULL);
}

int exec_echo(char *arg, ...)
{
    return execlp("/bin/echo", "echo", arg, NULL);
}

void pipe_process()
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        print_error("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        print_error("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        if (exec_ps("eaux") == -1)
        {
            print_error("[ERROR] exec_ps");
            exit(EXIT_FAILURE);
        }

    }
    else
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        if (exec_grep("^root") == -1)
        {
            print_error("[ERROR] exec_grep");
            exit(EXIT_FAILURE);
        }
    }

}

// TO DO: add echo if all is OK
