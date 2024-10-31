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
        exec_ps("eaux");
    }
    else
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        exec_grep("^root");
    }

}

// TO DO: add echo if all is OK