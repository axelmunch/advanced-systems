#include "executor.h"

int execute_single_command(char **args)
{
    if (args == NULL || args[0] == NULL)
        return EXIT_FAILURE;

    if (strcmp(args[0], "exit") == 0)
    {
        print_generic(STDOUT_FILENO, GREEN_COLOR "Bye! Thanks for using WinAxShell!\n" RESET_COLOR);
        exit(EXIT_SUCCESS);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        print_error("[ERROR] fork() failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) // child
    {
        if (execvp(args[0], args) == -1) // exec the command
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
    if (left == NULL || right == NULL)
        return EXIT_FAILURE;

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        print_error("[ERROR] pipe() failed");
        return EXIT_FAILURE;
    }

    pid_t left_pid = fork();
    if (left_pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        print_error("[ERROR] fork() failed");
        return EXIT_FAILURE;
    }

    if (left_pid == 0)
    {
        close(pipefd[0]);
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]);

        int status;
        if (left->op_type == OP_PIPE)
        {
            status = execute_pipe_command(left->left, left->right);
        }
        else
        {
            status = execute_single_command(left->args);
        }
        exit(status);
    }

    pid_t right_pid = fork();
    if (right_pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        print_error("[ERROR] fork() failed");
        return EXIT_FAILURE;
    }

    if (right_pid == 0)
    {
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            print_error("[ERROR] dup2() failed");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);

        int status;
        if (right->op_type == OP_PIPE)
        {
            status = execute_pipe_command(right->left, right->right);
        }
        else
        {
            status = execute_single_command(right->args);
        }
        exit(status);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    int left_status, right_status;
    waitpid(left_pid, &left_status, 0);
    waitpid(right_pid, &right_status, 0);

    return WIFEXITED(right_status) ? WEXITSTATUS(right_status) : EXIT_FAILURE;
}

int execute_command_tree(command_node_t *node)
{
    if (node == NULL)
        return EXIT_SUCCESS;

    int status = EXIT_SUCCESS;

    switch (node->op_type)
    {
    case OP_PIPE:
        status = execute_pipe_command(node->left, node->right);
        break;
    case OP_SEQ:
        status = execute_command_tree(node->left);
        if (status == EXIT_SUCCESS)
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
        status = execute_command_tree(node->left);
        break;
    case OP_NONE:
        status = execute_single_command(node->args);
        break;
    }
    return status;
}

void execute_command(char **args)
{
    if (args == NULL || args[0] == NULL)
        return;

    command_tree_t *tree = parse_command(args[0]);
    if (tree != NULL)
    {
        execute_command_tree(tree->root);
        free_command_tree(tree->root);
        free(tree);
    }
}