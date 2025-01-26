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

/**
 * @brief Intenal function to print the command tree structure for debugging purposes
 * @param node Command node
 * @param depth Depth of the tree
 * @return void
 */
static void print_command_tree(command_node_t *node, int depth)
{
    int show_debug = 1;

    if (!show_debug)
        return;

    if (!node)
        return;

    for (int i = 0; i < depth; i++)
        print_generic(STDERR_FILENO, "  ");

    print_generic(STDERR_FILENO, "Node: op_type=%d, args=[", node->op_type);

    if (node->args)
    {
        for (int i = 0; node->args[i] != NULL; i++)
            print_generic(STDERR_FILENO, "%s%s", i > 0 ? ", " : "", node->args[i] ? node->args[i] : "NULL");
    }
    print_generic(STDERR_FILENO, "]\n");

    if (node->left)
    {
        for (int i = 0; i < depth; i++)
            print_generic(STDERR_FILENO, "  ");

        print_generic(STDERR_FILENO, "Left child:\n");
        print_command_tree(node->left, depth + 1);
    }

    if (node->right)
    {
        for (int i = 0; i < depth; i++)
            print_generic(STDERR_FILENO, "  ");

        print_generic(STDERR_FILENO, "Right child:\n");
        print_command_tree(node->right, depth + 1);
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
        int child_status = custom_exec(args[0], args);
        if (child_status == -1)
        {
            print_error("[ERROR] %s", args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (!is_custom_command_main_process(args[0]))
        {
            int status;
            waitpid(pid, &status, 0);
            return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
        }
        else
        {
            return execute_custom_command_main_process(args[0], args) ? EXIT_SUCCESS : EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int execute_pipe_command(command_node_t *left, command_node_t *right)
{
    if (left == NULL || right == NULL)
    {
        print_error("[ERROR] Invalid pipe command");
        return EXIT_FAILURE;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        print_error("[ERROR] pipe() failed");
        return EXIT_FAILURE;
    }

    pid_t left_pid = fork();
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
            exit(EXIT_FAILURE);
        }
        safe_close(pipefd[1]);

        if (left->op_type == OP_PIPE)
        {
            exit(execute_pipe_command(left->left, left->right));
        }
        else
        {
            custom_exec(left->args[0], left->args);
            print_error("[ERROR] Failed to execute %s", left->args[0]);
            exit(EXIT_FAILURE);
        }
    }

    pid_t right_pid = fork();
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
            exit(EXIT_FAILURE);
        }
        safe_close(pipefd[0]);

        custom_exec(right->args[0], right->args);
        print_error("[ERROR] Failed to execute %s", right->args[0]);
        exit(EXIT_FAILURE);
    }

    safe_close(pipefd[0]);
    safe_close(pipefd[1]);

    int left_status, right_status;
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    return (WIFEXITED(left_status) && WIFEXITED(right_status)) ? EXIT_SUCCESS : EXIT_FAILURE;
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
        int child_status = custom_exec(node->args[0], node->args);
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

    int input_fd = -1;
    int output_fd = -1;

    switch (redirect_type)
    {
    case OP_REDIR_OUT:
        output_fd = safe_open(right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
    case OP_APPEND:
        output_fd = safe_open(right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;
    case OP_REDIR_IN:
    case OP_HEREDOC:
        input_fd = safe_open(right->args[0], O_RDONLY, 0);
        break;
    default:
        return EXIT_FAILURE;
    }

    if ((input_fd < 0 && (redirect_type == OP_REDIR_IN || redirect_type == OP_HEREDOC)) ||
        (output_fd < 0 && (redirect_type == OP_REDIR_OUT || redirect_type == OP_APPEND)))
    {
        print_error("[ERROR] Failed to open redirection file");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        if (input_fd >= 0)
            safe_close(input_fd);
        if (output_fd >= 0)
            safe_close(output_fd);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        if (input_fd >= 0)
        {
            if (dup2(input_fd, STDIN_FILENO) < 0)
            {
                print_error("[ERROR] Input redirection failed");
                exit(EXIT_FAILURE);
            }
            safe_close(input_fd);
        }

        if (output_fd >= 0)
        {
            if (dup2(output_fd, STDOUT_FILENO) < 0)
            {
                print_error("[ERROR] Output redirection failed");
                exit(EXIT_FAILURE);
            }
            safe_close(output_fd);
        }

        exit(execute_command_tree(left));
    }

    if (input_fd >= 0)
        safe_close(input_fd);
    if (output_fd >= 0)
        safe_close(output_fd);

    int status;
    waitpid(pid, &status, 0);

    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}

int custom_exec(char *command, char **args)
{
    // Custom commands
    if (is_custom_command(command))
    {
        execute_custom_command(command, args);
    }
    return execvp(command, args);
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
        if (node->left != NULL)
            status = execute_command_tree(node->left);
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
    print_command_tree(command_tree->root, 0);
    execute_command_tree(command_tree->root);
    free_command_node(command_tree->root);
    free_if_needed(command_tree);
    command_tree = NULL;
}
