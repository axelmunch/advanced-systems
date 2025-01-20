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

int interactive_mode(void) // TODO: refactor
{
    char input[MAX_INPUT];
    command_tree_t *command_tree = NULL;

    while (1)
    {
        print_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            print_generic(STDOUT_FILENO, "\n");
            break;
        }

        size_t input_len = strlen(input);

        // Check for input overflow
        if (input_len > 0 && input[input_len - 1] != '\n')
        {
            errno = EOVERFLOW;
            print_error("Input too long. Maximum allowed is %d characters.\n", MAX_INPUT - 1);
            while (getchar() != '\n' && !feof(stdin)); // Clear input buffer
            continue;
        }

        if (input_len > 0 && input[input_len - 1] == '\n')
            input[input_len - 1] = '\0';

        if (strlen(input) == 0)
            continue;

        if (strcmp(input, "exit") == 0) // exit command, to improve
        {
            print_generic(STDOUT_FILENO, GREEN_COLOR "Bye! Thanks for using WinAxShell!\n" RESET_COLOR);
            break;
        }

        execute_command(input, command_tree);
    }

    if (command_tree != NULL)
    {
        free_command_node(command_tree->root);
        free_if_needed(command_tree);
    }

    return EXIT_SUCCESS;
}

int batch_mode(int argc, char **argv)
{
    errno = ENOSYS;
    print_error("[ERROR] batch mode not implemented yet");
    return EXIT_FAILURE;
}

int main(int argc, char **argv)
{
    parse_options(argc, argv);
    check_requirements(argc, argv);
    show_parameters(get_verbose_mode());

    // Business logic, improve status handler
    if (argc == 1)
    {
        interactive_mode();
    }
    else
    {
        int status = batch_mode(argc, argv);
        return status;
    }

    return EXIT_SUCCESS;
}
