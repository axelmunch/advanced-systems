#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include "typedef.h"
#include "print.h"

/**
 * @brief Procedure checks if variable must be free(check: ptr != NULL)
 * @param void* to_free pointer to an allocated mem
 * @see man 3 free
 * @return void
 */
void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

/**
 * @brief Duplicate the option argument string (optarg)
 * @return void
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

/**
 * @brief Print help and exit
 */
void show_help(char **argv)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    exit(EXIT_FAILURE);
}

/**
 * @brief to show the binary parameters
 */
void show_parameters(bool verbose_mode)
{
    print("** PARAMS **\n%-8s: %d\n",
          "verbose", verbose_mode);
}

/**
 * @brief Checking binary requirements
 */
void check_requirements()
{
    return;
}

/**
 * @brief Parse binary options
 */
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

/**
 * @brief Binary main loop
 *
 * @return EXIT_SUCCESS if it exits successfully
 */
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
