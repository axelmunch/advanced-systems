#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include "constants.h"
#include "thread_sort.h"

/**
 * @brief Procedure checks if variable must be free (check: ptr != NULL)
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
 * @return char* duplicated string
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
    {"thread", required_argument, 0, 't'},
    {0, 0, 0, 0}};

/**
 * @brief Print help and exit
 * @param char** argv Arguments
 * @return void
 */
void show_help(char **argv)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    exit(EXIT_FAILURE);
}

/**
 * @brief Show the binary parameters in verbose mode
 * @param bool verbose_mode Verbose mode
 * @return void
 */
void show_parameters(bool verbose_mode)
{
    print("** PARAMS **\n%-8s: %d\n",
          "verbose", verbose_mode);
}

/**
 * @brief Checking binary requirements
 * @param int* num_threads Number of threads
 * @return void
 */
void check_requirements(int *num_threads)
{
    if (*num_threads <= 0)
    {
        print_generic(STDERR_FILENO, "[ERROR] Bad usage! See HELP [--help|-h]\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Parse binary options
 * @param int argc Number of arguments
 * @param char** argv Arguments
 * @return void
 */
void parse_options(int argc, char **argv)
{
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, BINARY_OPTION_STR, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 't':
            num_threads = atoi(optarg);
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

/**
 * @brief Binary main loop
 * @return EXIT_SUCCESS if it exits successfully
 */
int main(int argc, char **argv)
{
    min_max_t min_max;

    // Parsing binary options
    parse_options(argc, argv);

    // Checking binary requirements
    check_requirements(&num_threads);

    // Printing params if verbose mode is enabled
    show_parameters(get_verbose_mode());

    // Business logic
    initialize_array();
    sequential_search(&min_max, TIME_MSEC);
    threaded_search(&min_max, TIME_MSEC, num_threads);

    return EXIT_SUCCESS;
}
