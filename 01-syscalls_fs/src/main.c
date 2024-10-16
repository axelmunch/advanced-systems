#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include "constants.h"
#include "print.h"
#include "copy.h"

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
        str = strndup(optarg, MAX_PATH_LENGTH);
        if (str == NULL) 
        {
            perror(strerror(errno));
        }
    }
    return str;
}

/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 */
static struct option binary_opts[] = {
    {"help",    no_argument,       0, 'h'},
    {"verbose", no_argument,       0, 'v'},
    {"input",   required_argument, 0, 'i'},
    {"output",  required_argument, 0, 'o'},
    {0,         0,                 0,  0}
};

/**
 * Struct to store binary parameters
 */
typedef struct {
    char *input;
    char *output;
} binary_params_t;

/**
 * Checking binary requirements
 * (could be defined in a separate function)
 */
void check_requirements(binary_params_t *params) 
{
    if (params->input == NULL || params->output == NULL) 
    {
        print_error("Bad usage! See HELP [--help|-h]\n");
        free_if_needed(params->input);
        free_if_needed(params->output);
        exit(EXIT_FAILURE);
    }
}

/**
 * Function to print the binary parameters
 */
void print_parameters(const binary_params_t *params, bool verbose_mode) 
{
    print("** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
          "input", params->input,
          "output", params->output,
          "verbose", verbose_mode);
}

/**
 * Print help and exit
 */
void print_help(char **argv, binary_params_t *params) 
{
    print_generic(STDOUT, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    free_if_needed(params->input);
    free_if_needed(params->output);
    exit(EXIT_SUCCESS);
}

/**
 * Binary main loop
 *
 * \return 1 if it exits successfully
 */
int main(int argc, char **argv) 
{
    // Binary parameters initialization
    binary_params_t params = {0};

    // Parsing options
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, "hvi:o:", binary_opts, &opt_idx)) != -1) 
    {
        switch (opt) 
        {
        case 'i':
            // Input param
            if (optarg) 
                params.input = dup_optarg_str();
            break;
        case 'o':
            // Output param
            if (optarg)
                params.output = dup_optarg_str();
            break;
        case 'v':
            // Verbose mode
            set_verbose_mode(true);
            break;
        case 'h':
            print_help(argv, &params);
            break;
        default:
            break;
        }
    }

    // Checking binary requirements
    check_requirements(&params);

    // Printing params if verbose mode is enabled
    print_parameters(&params, get_verbose_mode());

    // Business logic must be implemented at this point
    copy(params.input, params.output);

    // Freeing allocated data
    free_if_needed(params.input);
    free_if_needed(params.output);

    return EXIT_SUCCESS;
}
