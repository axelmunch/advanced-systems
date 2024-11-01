#include "pipe.h"

int exec_ps(char *arg, ...)
{
    return execlp("/bin/ps", "ps", arg, NULL);
}

int exec_grep(char *arg, ...)
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
    pid_t ps_pid, grep_pid;
    int null_fd, status;

    if (pipe(pipe_fd) == -1)
    {
        print_error("pipe");
        exit(EXIT_FAILURE);
    }

    ps_pid = fork();
    if (ps_pid == -1)
    {
        print_error("fork");
        exit(EXIT_FAILURE);
    }
    else if (ps_pid == 0)
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
        grep_pid = fork();
        if (grep_pid == -1)
        {
            print_error("fork");
            exit(EXIT_FAILURE);
        }
        else if (grep_pid == 0)
        {
            close(pipe_fd[1]);
            dup2(pipe_fd[0], STDIN_FILENO);
            null_fd = open("/dev/null", O_WRONLY);

            if (null_fd == -1)
            {
                print_error("[ERROR] open");
                exit(EXIT_FAILURE);
            }

            dup2(null_fd, STDOUT_FILENO);
            close(null_fd);

            if (exec_grep("^root") == -1)
            {
                print_error("[ERROR] exec_grep");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            waitpid(grep_pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            {
                print_generic(STDOUT_FILENO, "root est connecté\n");
            }
            else
            {
                print_generic(STDOUT_FILENO, "root n'est pas connecté\n");
            }
        }
    }
}

// TO DO: add echo if all is OK
