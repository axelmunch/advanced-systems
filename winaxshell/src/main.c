#include "main.h"

int interactive_mode(void) // TODO: refactor
{
    char input[MAX_INPUT_LENGTH];
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
            print_error("Input too long. Maximum allowed is %d characters.\n", MAX_INPUT_LENGTH - 1);
            while (getchar() != '\n' && !feof(stdin)); // Clear input buffer
            continue;
        }

        if (input_len > 0 && input[input_len - 1] == '\n')
            input[input_len - 1] = '\0';

        if (strlen(input) == 0)
            continue;

        if (strcmp(input, "exit") == 0) // exit command, to improve so we can exit with error code
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
    free_aliases();

    return EXIT_SUCCESS;
}

int batch_mode(int argc, char **argv)
{
    command_tree_t *command_tree = NULL;
    int status = EXIT_SUCCESS;

    char *input = get_requested_command();
    if (input != NULL)
        status = execute_command(input, command_tree);

    return status;
}

int main(int argc, char **argv)
{
    parse_options(argc, argv);
    check_requirements(argc, argv);
    show_parameters(get_verbose_mode());

    int status = EXIT_SUCCESS;

    if (argc == 1)
        status = interactive_mode();
    else
        status = batch_mode(argc, argv);

    return status;
}
