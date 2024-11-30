#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include "constants.h"
#include "print.h"
#include "copy.h"
#include "ls_like.h"
#include "reverse.h"
#include "buffered_io.h"
#include "typedef.h"

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
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"reverse", no_argument, 0, 'r'},
    {"list", no_argument, 0, 'l'},
    {"copy", no_argument, 0, 'c'},
    {0, 0, 0, 0}};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char *binary_optstr = "hvcrli:o:";

/**
 * Checking binary requirements
 * List mode does not require input and output
 * Other modes require both input and output
 */
void check_requirements(binary_params_t *params, command_mode mode)
{
    if (mode == LIST_MODE)
    {
        return;
    }

    if (params->input == NULL || params->output == NULL)
    {
        print_generic(STDERR_FILENO, "Bad usage! See HELP [--help|-h]\n");
        free_if_needed(params->input);
        free_if_needed(params->output);
        exit(EXIT_FAILURE);
    }
}

/**
 * Function to show the binary parameters
 */
void show_parameters(const binary_params_t *params, bool verbose_mode)
{
    print("** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
          "input", params->input,
          "output", params->output,
          "verbose", verbose_mode);
}

/**
 * Print help and exit
 */
void show_help(char **argv, binary_params_t *params)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
    free_if_needed(params->input);
    free_if_needed(params->output);
    exit(EXIT_FAILURE);
}

/**
 * Parse binary options
 */
void parse_options(int argc, char **argv, binary_params_t *params, command_mode *mode)
{
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'i':
            // Input param
            if (optarg)
                params->input = dup_optarg_str();
            break;
        case 'o':
            if (optarg)
                params->output = dup_optarg_str();
            break;
        case 'c':
            *mode = COPY_MODE;
            break;
        case 'r':
            *mode = REVERSE_MODE;
            break;
        case 'l':
            *mode = LIST_MODE;
            break;
        case 'v':
            set_verbose_mode(true);
            break;
        case 'h':
            show_help(argv, params);
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
    // Binary parameters initialization
    binary_params_t params = {0};
    command_mode mode = UNDEFINED_MODE;

    // Parsing binary options
    parse_options(argc, argv, &params, &mode);

    // Checking binary requirements
    check_requirements(&params, mode);

    // Printing params if verbose mode is enabledm
    show_parameters(&params, get_verbose_mode());

    // Business logic must be implemented at this point
    switch (mode)
    {
    case COPY_MODE:
        copy(params.input, params.output);
        break;
    case REVERSE_MODE:
        reverse(params.input, params.output);
        break;
    case LIST_MODE:
        ls_like(params.input);
        break;
    default:
        print_error("Undefined mode\n");
        break;
    }

    // Freeing allocated data
    free_if_needed(params.input);
    free_if_needed(params.output);

    // Buffered IO
    print("BUFFERED IO\n");
    print("----------------\n");
    // Write
    print("WRITING\n");
    print("----------------\n");
    FICHIER *file_out = my_open("output.txt", "w");
    if (file_out)
    {
        my_putc('T', file_out);
        my_putc('e', file_out);
        my_putc('s', file_out);
        my_putc('t', file_out);
        my_putc('\n', file_out);
        my_putc('b', file_out);
        my_putc('u', file_out);
        my_putc('f', file_out);
        my_putc('f', file_out);
        my_putc('e', file_out);
        my_putc('r', file_out);
        my_putc('e', file_out);
        my_putc('d', file_out);
        my_putc(' ', file_out);
        my_putc('I', file_out);
        my_putc('O', file_out);
        my_close(file_out);
    }
    print("----------------\n");
    // Read
    print("READING\n");
    print("----------------\n");
    FICHIER *file_in = my_open("output.txt", "r");
    if (file_in)
    {
        int c;
        while ((c = my_getc(file_in)) != EOF)
        {
            print("%c", c);
        }
        print("\n");
        my_close(file_in);
    }
    print("----------------\n");

    return EXIT_SUCCESS;
}
