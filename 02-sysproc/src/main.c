#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include "constants.h"
#include "fork_yourself.h"
#include "redirect.h"
#include "typedef.h"
#include "pipe.h"

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
    {"redirect", required_argument, 0, 'r'},
    {"pipe", no_argument, 0, 'p'},
    {0, 0, 0, 0}};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * see man 3 getopt_long or getopt
 */
const char *binary_optstr = "hvfr:p";

/**
 * Print help and exit
 */
void show_help(char **argv)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    exit(EXIT_FAILURE);
}

/**
 * Checking binary requirements
 * @param mode The command mode
 * @param redirect_arg The argument for redirect mode
 * @return void
 */
int check_requirements(command_mode mode, char *program_name)
{
    if (mode == UNDEFINED_MODE)
    {
        print_error("[ERROR] No valid mode specified! See HELP [--help|-h].");
        return EXIT_FAILURE;
    }

    if (mode == REDIRECT_MODE && program_name == NULL)
    {
        print_error("[ERROR] Redirect mode requires an argument! See HELP [--help|-h]");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
void parse_options(int argc, char **argv, command_mode *mode, char **program_name)
{
    int opt = -1;
    int opt_idx = -1;

    *mode = UNDEFINED_MODE;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'f':
            *mode = FORK_MODE;
            break;
        case 'r':
            *mode = REDIRECT_MODE;
            if (optarg)
                *program_name = dup_optarg_str();
            break;
        case 'p':
            *mode = PIPE_MODE;
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
 * Binary main loop
 *
 * \return 1 if it exits successfully
 */
int main(int argc, char **argv)
{
    command_mode mode;
    char *program_name = NULL;

    // Parsing binary options
    parse_options(argc, argv, &mode, &program_name);

    // Checking binary requirements
    check_requirements(mode, program_name);

    // Printing params if verbose mode is enabled
    show_parameters(get_verbose_mode());

    // Running binary depending on the mode
    switch (mode)
    {
    case FORK_MODE:
        fork_yourself();
        break;
    case REDIRECT_MODE:
        redirect(program_name, STDOUT_FILENO);
        // redirect(program_name, STDERR_FILENO);
        break;
    case PIPE_MODE:
        pipe_process();
        break;
    default:
        break;
    }

    return EXIT_SUCCESS;
}
