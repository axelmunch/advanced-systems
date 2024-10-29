#include "fork_yourself.h"

int fork_yourself(void)
{
    int status;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return EXIT_FAILURE;
    }
    else if (pid == 0)
    {
        printf("[INFO] Child process: PID=%d, PPID=%d\n", getpid(), getppid());
        exit(getpid() % 10);
    }
    else
    {
        pid_t child_pid = wait(&status);
        printf("[INFO] Parent process: Child_PID=%d\n", child_pid);
        printf("[SUCCESS] Child exited with code: %d\n", WEXITSTATUS(status));
    }

    return EXIT_SUCCESS;
}