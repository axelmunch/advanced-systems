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

/**
 * @brief Handle the ps command
 * @return void
 * @param pipe_fd[2] Pipe file descriptors
 *
 * This function will close the read end of the pipe, duplicate the write end of the pipe to the STDOUT and close the write end of the pipe.
 */
void handle_ps(int pipe_fd[2])
{
    close(pipe_fd[0]);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);

    if (exec_ps("aux") == -1)
    {
        print_error("[ERROR] exec_ps");
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Handle the grep command
 * @return void
 * @param pipe_fd[2] Pipe file descriptors
 *
 * This function will close the write end of the pipe, duplicate the read end of the pipe to the STDIN and close the read end of the pipe.
 */
void handle_grep(int pipe_fd[2])
{
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);

    int null_fd = open("/dev/null", O_WRONLY);
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


/**
 * @brief Handle the parent process
 * @return void
 * @param grep_pid PID of the grep process
 *
 * This function will wait for the grep process to finish and check if the exit status is 0. If it is, it will print "root est connecté", otherwise it will print "root n'est pas connecté".
 */
void handle_parent(pid_t grep_pid)
{
    int status;
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

/**
 * @brief Pipe process
 * @return void
 *
 * This function will create a pipe, fork two processes, one for the ps command and one for the grep command. The parent process will wait for the grep process to finish and print the result.
 */
void pipe_process()
{
    int pipe_fd[2];
    pid_t ps_pid, grep_pid;

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
        handle_ps(pipe_fd);
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
            handle_grep(pipe_fd);
        }
        else
        {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            handle_parent(grep_pid);
        }
    }
}
