#include "main.h"

/**
 * @brief Binary options declaration (must end with {0,0,0,0})
 */
static struct option binary_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
    {"command", required_argument, 0, 'c'},
    {0, 0, 0, 0}};

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

char *dup_optarg_str()
{
    char *str = NULL;
    if (optarg != NULL)
    {
        str = strndup(optarg, MAX_INPUT);
        if (str == NULL)
        {
            print_error("");
        }
    }
    return str;
}

void show_help(char **argv)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    exit(EXIT_FAILURE);
}

void show_parameters(bool verbose_mode)
{
    print("** PARAMS **\n%-8s: %d\n",
          "verbose", verbose_mode);
}

void check_requirements(int argc, char **argv)
{
    return;
}

void parse_options(int argc, char **argv)
{
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, BINARY_OPTION_STR, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'c':
            break;
        case 'v':
            set_verbose_mode(true);
            break;
        case 'h':
            show_help(argv);
            break;
        default:
            break;
        }
    }
}

void print_prompt()
{
    char cwd[MAX_INPUT];
    char *username = getenv("USER");

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        print_generic(STDOUT_FILENO, "%s%s@%s%s:%s%s%s$ ", GREEN_COLOR, username, SHELL_NAME, RESET_COLOR, BLUE_COLOR, cwd, RESET_COLOR);
    }
    else
    {
        print_error("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

void interactive_mode()
{
    while (1)
    {
        print_prompt();
        char input[MAX_INPUT];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        char** command_arguments = parse_command(input);
        if (command_arguments != NULL)
        {
            execute_command(command_arguments);
        }
    }
}

void batch_mode(int argc, char **argv)
{
    return;
}

int main(int argc, char **argv)
{
    // Parsing binary options
    parse_options(argc, argv);

    // Checking binary requirements
    check_requirements(argc, argv);

    // Printing params if verbose mode is enabled
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
