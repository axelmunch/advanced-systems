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

void pipe_process(int pipe_fd[2], pid_t pid)
{    
    if (pipe(pipe_fd) == -1)
    {
        print_error("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1)
    {
        print_error("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child implementation */
        
    }
    else
    {
        /* Parent implementation */
    }

}