#include "main.h"

void print_prompt()
{
    char cwd[MAX_INPUT];
    char *username = getenv("USER");

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        print_generic(STDOUT_FILENO, GREEN_COLOR "%s@%s" RESET_COLOR ":" BLUE_COLOR "%s\n" RESET_COLOR "$ ", username, SHELL_NAME, cwd);
    }
    else
    {
        print_error("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

void interactive_mode()
{
    char input[MAX_INPUT];

    while (1)
    {
        print_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            print_generic(STDOUT_FILENO, "\n");
            break;
        }

        size_t input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n')
            input[input_len - 1] = '\0';

        if (strlen(input) == 0)
            continue;

        command_tree_t *command_tree = parse_command(input);
        if (command_tree == NULL)
        {
            print_error("[ERROR] Failed to parse command");
            continue;
        }

        execute_command_tree(command_tree->root);

        free_command_tree(command_tree->root);
        free(command_tree);
    }
}

void batch_mode(int argc, char **argv)
{
    errno = ENOSYS;
    print_error("[ERROR] batch mode not implemented yet");
    return;
}

int main(int argc, char **argv)
{
    parse_options(argc, argv);
    check_requirements(argc, argv);
    show_parameters(get_verbose_mode());

    // Business logic
    if (argc == 1)
    {
        interactive_mode();
    }
    else
    {
        batch_mode(argc, argv);
    }

    return EXIT_SUCCESS;
}
