#include "executor.h"

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
            print_error("[ERROR] execvp() failed");
            exit(EXIT_FAILURE);
        }
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

int execute_pipe_command(command_node_t *left, command_node_t *right)
{
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
        close(pipefd[0]);
        close(pipefd[1]);
        return EXIT_FAILURE;
    }

    if (left_pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);

        if (left->op_type == OP_PIPE)
            status = execute_pipe_command(left->left, left->right);
        else
            status = execute_single_command(left->args);
        exit(status);
    }

    right_pid = fork();
    if (right_pid == -1)
    {
        print_error("[ERROR] fork() failed");
        close(pipefd[0]);
        close(pipefd[1]);
        kill(left_pid, SIGTERM);
        waitpid(left_pid, NULL, 0);
        return EXIT_FAILURE;
    }

    if (right_pid == 0)
    {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);

        if (right->op_type == OP_PIPE)
            status = execute_pipe_command(right->left, right->right);
        else
            status = execute_single_command(right->args);

        exit(status);
    }

    close(pipefd[0]);
    close(pipefd[1]);

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
        int setsid_status = setsid();
        if (setsid_status == -1)
        {
            print_error("[ERROR] setsid() failed");
            exit(EXIT_FAILURE);
        }
        int child_status = execvp(node->args[0], node->args);
        if (child_status == -1)
        {
            print_error("[ERROR] execvp() failed");
            exit(EXIT_FAILURE);
        }
    }

    print_generic(STDOUT_FILENO, "[%d] %s running in background.\n", pid, node->args[0]);
    signal(SIGCHLD, SIG_IGN);
    return EXIT_SUCCESS;
}

int execute_redirection_command(command_node_t *left, command_node_t *right, operator_t redirect_type)
{
    if (left == NULL || right == NULL || right->args == NULL || right->args[0] == NULL)
        return EXIT_FAILURE;

    int redirect_fd;
    switch (redirect_type)
    {
    case OP_REDIR_OUT:
        redirect_fd = open(right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
    case OP_APPEND:
        redirect_fd = open(right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;
    case OP_REDIR_IN:
        redirect_fd = open(right->args[0], O_RDONLY);
        break;
    default:
        redirect_fd = -1;
    }
    if (redirect_fd < 0)
    {
        print_error("[ERROR] open() failed");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        close(redirect_fd);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        if (redirect_type == OP_REDIR_OUT || redirect_type == OP_APPEND)
        {
            if (dup2(redirect_fd, STDOUT_FILENO) < 0)
            {
                print_error("[ERROR] dup2() failed");
                close(redirect_fd);
                exit(EXIT_FAILURE);
            }
        }
        else if (redirect_type == OP_REDIR_IN)
        {
            if (dup2(redirect_fd, STDIN_FILENO) < 0)
            {
                print_error("[ERROR] dup2() failed");
                close(redirect_fd);
                exit(EXIT_FAILURE);
            }
        }
        close(redirect_fd);
        exit(execute_single_command(left->args));
    }
    close(redirect_fd);

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
    case OP_PIPE: // OK
        status = execute_pipe_command(node->left, node->right);
        break;
    case OP_SEQ: // TODO: handle operator of the right node, be able to chained operators
        execute_command_tree(node->left);
        if (node->right != NULL)
            status = execute_command_tree(node->right);
        break;
    case OP_AND: // OK
        status = execute_command_tree(node->left);
        if (status == EXIT_SUCCESS)
            status = execute_command_tree(node->right);
        break;
    case OP_OR: // OK
        status = execute_command_tree(node->left);
        if (status != EXIT_SUCCESS)
            status = execute_command_tree(node->right);
        break;
    case OP_BG: // TODO
        status = execute_background_command(node->left);
        break;
    case OP_REDIR_OUT:
    case OP_REDIR_IN:
    case OP_APPEND:
        status = execute_redirection_command(node->left, node->right, node->op_type);
        break;
    case OP_HEREDOC: // TODO
        break;
    case OP_NONE: // OK
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
    command_tree = parse_command(input);
    if (command_tree == NULL)
    {
        print_error("[ERROR] Failed to parse command");
        return;
    }
    // TODO: add the command tree to the history
    // Handle if command_tree is a built-in command
    execute_command_tree(command_tree->root);
    free_command_node(command_tree->root);
    free_if_needed(command_tree);
    command_tree = NULL;
}