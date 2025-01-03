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
        str = strndup(optarg, MAX_INPUT_LENGTH);
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

void check_requirements()
{
    return;
}

void parse_options(int argc, char **argv)
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

void interactive_mode()
{
    return;
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

    return EXIT_SUCCESS;
}
