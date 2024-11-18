#include "fork_yourself.h"

/**
 * @brief Fork yourself
 * @return int
 *
 * This function will fork a child process and print the PID and PPID of the child and the PID of the parent.
 */
int fork_yourself(void)
{
    int status;

    pid_t pid = fork();

    if (pid == -1)
    {
        print_error("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        print_generic(STDOUT_FILENO, "\tChild: PID=%d, PPID=%d\n", getpid(), getppid());
        exit(getpid() % 10);
    }
    else
    {
        pid_t child_pid = wait(&status);
        print_generic(STDOUT_FILENO, "Parent: Child_PID=%d\n", child_pid);
        print_generic(STDOUT_FILENO, "Child exited with code: %d\n", WEXITSTATUS(status));
    }

    return EXIT_SUCCESS;
}
