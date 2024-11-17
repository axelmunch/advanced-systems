#include "redirect.h"

void exec_program(char *program_name)
{
    int ret;
    ret = execlp(program_name, program_name, NULL);
    if (ret == -1)
    {
        print_error("[ERROR] execlp");
        exit(EXIT_FAILURE);
    }
}

void open_temp(int redirect_fd)
{
    int tmp_fd;

    char template[] = "/tmp/proc-exercise-XXXXXX";

    tmp_fd = mkstemp(template);
    if (tmp_fd == -1)
    {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }
    print("[INFO] Created temp file: %s with file descriptor %d\n", template, tmp_fd);

    tmp_fd = dup2(tmp_fd, redirect_fd);
    if (tmp_fd == -1)
    {
        perror("dup2");
        close(tmp_fd);
        exit(EXIT_FAILURE);
    }
    print("[INFO] New file descriptor of temp file: %d\n", tmp_fd);
}

int show_msg(char *message)
{
    print_generic(STDOUT_FILENO, "Program first argument: %s\n", message);
    return EXIT_SUCCESS;
}

void process_child(char *program_name, int redirect_fd)
{
    print("[INFO] Child process: PID=%d\n", getpid());
    close(redirect_fd);
    open_temp(redirect_fd);
    exec_program(program_name);
}

void process_parent()
{
    print("[INFO] Parent process: PID=%d\n", getpid());
    wait(NULL);
    print_generic(STDOUT_FILENO, "That's All Folks!\n");
    exit(EXIT_SUCCESS);
}

void redirect(char *program_name, int redirect_fd)
{
    show_msg(program_name);

    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        print_error("[ERROR] fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        process_child(program_name, redirect_fd);
    }
    else
    {
        process_parent();
    }

    exit(EXIT_SUCCESS);
}
