#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "helper.h"
#include "constants.h"

void redirect_setup(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(helper, .init = redirect_setup);

Test(helper, test_get_requested_command)
{
    cr_assert_null(get_requested_command());
}

Test(helper, test_dup_optarg_str)
{
    optarg = "test_command";
    char *result = dup_optarg_str();
    cr_assert_str_eq(result, "test_command");
    free(result);

    optarg = NULL;
    result = dup_optarg_str();
    cr_assert_null(result);
}

Test(helper, test_show_help)
{
    char *argv[] = {"program_name", NULL};
    show_help(argv);
    fflush(stdout);
    cr_assert_stdout_eq_str("USAGE: program_name " USAGE_SYNTAX "\n\n" USAGE_PARAMS "\n");
}

Test(helper, test_check_requirements)
{
    char *argv[] = {"program_name", NULL};
    check_requirements(1, argv);
    cr_assert(1); // Should not crash
}

Test(helper, test_parse_options_command)
{
    char *argv[] = {"program", "-c", "test_cmd", NULL};
    int argc = 3;
    optind = 1;
    parse_options(argc, argv);
    cr_assert_str_eq(get_requested_command(), "test_cmd");
}

Test(helper, test_parse_options_command_null)
{
    char *argv[] = {"program", "-c", NULL};
    int argc = 2;
    optind = 1;
    parse_options(argc, argv);
    cr_assert_null(get_requested_command(), "Requested command should be NULL");
}

Test(helper, test_parse_options_verbose)
{
    char *argv[] = {"program", "-v", NULL};
    int argc = 2;
    optind = 1;
    parse_options(argc, argv);
    cr_assert_eq(get_verbose_mode(), 1, "Verbose mode should be enabled");
    set_verbose_mode(false);
}

Test(helper, test_parse_options_help)
{
    char *argv[] = {"program", "-h", NULL};
    int argc = 2;
    optind = 1;
    parse_options(argc, argv);
    fflush(stdout);
    cr_assert_stdout_eq_str("USAGE: program " USAGE_SYNTAX "\n\n" USAGE_PARAMS "\n");
}

Test(helper, test_parse_options_invalid)
{
    char *argv[] = {"program", "-x", NULL};
    int argc = 2;
    optind = 1;
    parse_options(argc, argv);
    cr_assert(1); // Should not crash
}