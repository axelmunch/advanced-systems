#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include "constants.h"
#include "fork_yourself.h"

typedef enum
{
    FORK_MODE = 0,
    REDIRECT_MODE,
    PIPE_MODE,
    UNKNOW_MODE
} command_mode;

/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

/**
 * Duplicate the option argument string (optarg)
 */
char *dup_optarg_str()
{
    char *str = NULL;
    if (optarg != NULL)
    {
        str = strndup(optarg, STR_SIZE);
        if (str == NULL)
        {
            print_error("");
        }
    }
    return str;
}

/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 */
static struct option binary_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
    {"fork-yourself", no_argument, 0, 'f'},
    {"redirect", no_argument, 0, 'r'},
    {"pipe", no_argument, 0, 'p'},
    {0, 0, 0, 0}};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char *binary_optstr = "hvfrpi:o:";

/**
 * Print help and exit
 */
void show_help(char **argv)
{
    print_generic(STDOUT, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    exit(EXIT_FAILURE);
}

/**
 * Function to show the binary parameters
 */
void show_parameters(bool verbose_mode)
{
    print("** PARAMS **\n%-8s: %d\n",
          "verbose", verbose_mode);
}

/**
 * Parse binary options
 */
void parse_options(int argc, char **argv, command_mode *mode)
{
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'f':
            *mode = FORK_MODE;
            break;
        case 'r':
            *mode = REDIRECT_MODE;
            break;
        case 'p':
            *mode = PIPE_MODE;
            break;
        case 'v':
            // Verbose mode
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

/**
 * Binary main loop
 *
 * \return 1 if it exits successfully
 */
int main(int argc, char **argv)
{
    command_mode mode = UNKNOW_MODE;

    // Parsing binary options
    parse_options(argc, argv, &mode);

    // Printing params if verbose mode is enabled
    show_parameters(get_verbose_mode());

    // Running binary depending on the mode
    switch (mode)
    {
    case FORK_MODE:
        fork_yourself();
        break;
    case REDIRECT_MODE:
        break;
    case PIPE_MODE:
        break;
    default:
        print_error("Unknown mode");
        exit(EXIT_FAILURE);
        break;
    }

    return EXIT_SUCCESS;
}
