#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <getopt.h>
#include "../include/constants.h"
#include "../include/print.h"
#include "../include/copy.h"


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
    if (to_free != NULL) free(to_free);
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
    char* str = NULL;

    if (optarg != NULL)
    {
        str = strndup(optarg, MAX_PATH_LENGTH);

        // Checking if ERRNO is set
        if (str == NULL)
            perror(strerror(errno));
    }

    return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] =
{
    { "help",    no_argument,       0, 'h' },
    { "verbose", no_argument,       0, 'v' },
    { "input",   required_argument, 0, 'i' },
    { "output",  required_argument, 0, 'o' },
    { 0,         0,                 0,  0  }
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char* binary_optstr = "hvi:o:";



/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{
    /**
     * Binary variables
     * (could be defined in a structure)
     */
    char* bin_input_param = NULL;
    char* bin_output_param = NULL;

    // Parsing options
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
            case 'i':
            // Input param
            if (optarg)
            {
                bin_input_param = dup_optarg_str();
            }
            break;
            case 'o':
            // Output param
            if (optarg)
            {
                bin_output_param = dup_optarg_str();
            }
            break;
            case 'v':
            // Verbose mode
            set_verbose_mode(true);
            break;
            case 'h':
            print("USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);

            free_if_needed(bin_input_param);
            free_if_needed(bin_output_param);

            exit(EXIT_SUCCESS);
            default :
            break;
        }
    }

    /**
     * Checking binary requirements
     * (could defined in a separate function)
     */
    if (bin_input_param == NULL || bin_output_param == NULL)
    {
        print_error("Bad usage! See HELP [--help|-h]\n");
        // Freeing allocated data
        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
        // Exiting with a failure ERROR CODE (== 1)
        exit(EXIT_FAILURE);
    }


    // Printing params
    print("** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
            "input",   bin_input_param,
            "output",  bin_output_param,
            "verbose", get_verbose_mode());

    // Business logic must be implemented at this point

    copy(bin_input_param, bin_output_param);

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);


    return EXIT_SUCCESS;
}
