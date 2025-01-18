#include "executor.h"

/**
 * @brief Internal signal handler for SIGCHLD
 * @param signo Signal number
 * @return void
 */
static void sigchld_handler(int signo)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (WIFEXITED(status))
            print_generic(STDOUT_FILENO, "[%d] Process exited with status %d.\n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            print_generic(STDOUT_FILENO, "[%d] Process killed by signal %d.\n", pid, WTERMSIG(status));
    }
}

int execute_single_command(char **args)
{
    if (args == NULL || args[0] == NULL)
        return EXIT_FAILURE;

    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        int child_status = execvp(args[0], args);
        if (child_status == -1)
        {
            print_error("[ERROR] %s", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

int execute_pipe_command(command_node_t *left, command_node_t *right)
{
    if (left == NULL || right == NULL)
    {
        print_error("[ERROR] Null command node passed to execute_pipe_command()");
        return EXIT_FAILURE;
    }

    int pipefd[2];
    pid_t left_pid, right_pid;
    int status = EXIT_FAILURE;

    if (pipe(pipefd) == -1)
    {
        print_error("[ERROR] pipe() failed");
        return EXIT_FAILURE;
    }

    left_pid = fork();
    if (left_pid == -1)
    {
        print_error("[ERROR] fork() failed");
        safe_close(pipefd[0]);
        safe_close(pipefd[1]);
        return EXIT_FAILURE;
    }

    if (left_pid == 0)
    {
        safe_close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            safe_close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        safe_close(pipefd[1]);

        if (left->op_type == OP_PIPE)
        {
            status = execute_pipe_command(left->left, left->right);
            exit(status);
        }
        else
        {
            status = execvp(left->args[0], left->args);
            if (status == -1)
            {
                print_error("[ERROR] %s", left->args[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    right_pid = fork();
    if (right_pid == -1)
    {
        print_error("[ERROR] fork() failed");
        safe_close(pipefd[0]);
        safe_close(pipefd[1]);
        kill(left_pid, SIGTERM);
        waitpid(left_pid, NULL, 0);
        return EXIT_FAILURE;
    }

    if (right_pid == 0)
    {
        safe_close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            safe_close(pipefd[0]);
            exit(EXIT_FAILURE);
        }
        safe_close(pipefd[0]);

        status = execvp(right->args[0], right->args);
        if (status == -1)
        {
            print_error("[ERROR] %s", right->args[0]);
            exit(EXIT_FAILURE);
        }
    }

    safe_close(pipefd[0]);
    safe_close(pipefd[1]);

    int left_status, right_status;
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    return WIFEXITED(right_status) ? WEXITSTATUS(right_status) : EXIT_FAILURE;
}

int execute_background_command(command_node_t *node)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        int child_status = execvp(node->args[0], node->args);
        if (child_status == -1)
        {
            print_error("[ERROR] %s", node->args[0]);
            exit(EXIT_FAILURE);
        }
    }

    print_generic(STDOUT_FILENO, "[%d] %s running in background.\n", pid, node->args[0]);
    signal(SIGCHLD, sigchld_handler);
    return EXIT_SUCCESS;
}

int execute_redirection_command(command_node_t *left, command_node_t *right, operator_t redirect_type)
{
    if (left == NULL || right == NULL)
        return EXIT_FAILURE;

    int redirect_fd;
    switch (redirect_type)
    {
    case OP_REDIR_OUT:
        redirect_fd = safe_open(right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
    case OP_APPEND:
        redirect_fd = safe_open(right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;
    case OP_REDIR_IN:
    case OP_HEREDOC:
        redirect_fd = safe_open(right->args[0], O_RDONLY, 0);
        break;
    default:
        redirect_fd = -1;
    }
    if (redirect_fd < 0)
    {
        print_error("[ERROR] safe_open() failed");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        safe_close(redirect_fd);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        if (redirect_type == OP_REDIR_OUT || redirect_type == OP_APPEND)
        {
            if (dup2(redirect_fd, STDOUT_FILENO) < 0)
            {
                print_error("[ERROR] dup2() failed");
                safe_close(redirect_fd);
                exit(EXIT_FAILURE);
            }
        }
        else if (redirect_type == OP_REDIR_IN || redirect_type == OP_HEREDOC)
        {
            if (dup2(redirect_fd, STDIN_FILENO) < 0)
            {
                print_error("[ERROR] dup2() failed");
                safe_close(redirect_fd);
                exit(EXIT_FAILURE);
            }
        }
        safe_close(redirect_fd);
        exit(execute_single_command(left->args));
    }
    safe_close(redirect_fd);

    int status;
    waitpid(pid, &status, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

int execute_command_tree(command_node_t *node)
{
    if (node == NULL)
        return EXIT_SUCCESS;

    int status;

    switch (node->op_type)
    {
    case OP_PIPE:
        status = execute_pipe_command(node->left, node->right);
        break;
    case OP_SEQ:
        execute_command_tree(node->left);
        if (node->right != NULL)
            status = execute_command_tree(node->right);
        break;
    case OP_AND:
        status = execute_command_tree(node->left);
        if (status == EXIT_SUCCESS)
            status = execute_command_tree(node->right);
        break;
    case OP_OR:
        status = execute_command_tree(node->left);
        if (status != EXIT_SUCCESS)
            status = execute_command_tree(node->right);
        break;
    case OP_BG:
        status = execute_background_command(node->left);
        break;
    case OP_REDIR_OUT:
    case OP_REDIR_IN:
    case OP_APPEND:
    case OP_HEREDOC:
        status = execute_redirection_command(node->left, node->right, node->op_type);
        break;
    case OP_NONE:
        status = execute_single_command(node->args);
        break;
    default:
        status = EXIT_FAILURE;
        break;
    }

    return status;
}

void execute_command(char *input, command_tree_t *command_tree)
{
    add_history_entry(input);
    command_tree = parse_command(input);
    if (command_tree == NULL)
    {
        print_error("[ERROR] Failed to parse command");
        return;
    }
    // TODO: add the command tree to the history
    // Check if command_tree is a built-in command
    execute_command_tree(command_tree->root);
    free_command_node(command_tree->root);
    free_if_needed(command_tree);
    command_tree = NULL;
}
