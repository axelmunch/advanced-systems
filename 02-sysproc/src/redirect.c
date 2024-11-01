#include "redirect.h"

void open_temp()
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

    tmp_fd = dup2(tmp_fd, STDOUT_FILENO);
    if (tmp_fd == -1)
    {
        perror("dup2");
        close(tmp_fd);
        exit(EXIT_FAILURE);
    }
    print("[INFO] New file descriaptor of temp file: %d\n", tmp_fd);
}

int show_msg(char *message)
{
    print_generic(STDOUT_FILENO, "Program first argument: %s\n", message);
    return EXIT_SUCCESS;
}

void process_child(char *program_name)
{
    print("[INFO] Child process: PID=%d\n", getpid());
    close(STDOUT_FILENO);
    open_temp();
    execlp(program_name, program_name, NULL);
}

void process_parent()
{
    print("[INFO] Parent process: PID=%d\n", getpid());
    wait(NULL);
    print_generic(STDOUT_FILENO, "That's All Folks!\n");
    exit(EXIT_SUCCESS);
}

void _redirect_stdout(char *program_name)
{
    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        print_error("[ERROR] fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        process_child(program_name);
    }
    else
    {
        process_parent();
    }
}

void _redirect_stderr(char *program_name)
{
}

void redirect(char *program_name)
{
    show_msg(program_name);
    _redirect_stdout(program_name);

    exit(EXIT_SUCCESS);
}
