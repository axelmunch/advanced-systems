#include "main.h"

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

/**
 * @brief Binary options declaration (must end with {0,0,0,0})
 */
static struct option binary_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
    {0, 0, 0, 0}};

/**
 * @brief Binary options string (linked to option declaration)
 * @see man 3 getopt_long or getopt
 */
const char *binary_optstr = "hv";

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

void check_requirements() // for batch mode
{
    return;
}

void parse_options(int argc, char **argv) // for batch mode
{
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
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

        parse_command(input);
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
    check_requirements();

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
