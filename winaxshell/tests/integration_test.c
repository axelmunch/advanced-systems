#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/redirect.h>
#include "executor.h"
#include "parser.h"

void integration_setup(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

TestSuite(integration, .init = integration_setup);

Test(integration, test_chained_pipes)
{
    char *input = "ls -l | head -n 5 | head -n 4 | head -n 3 | tail -n 1 | wc -l";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Failed to parse command tree");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing chained pipes should succeed");

    FILE *output = cr_get_redirected_stdout();
    char* expected_output = "1\n";
    char buffer[256];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '1'");
}

Test(integration, test_chained_commands)
{
    char *input = "ls -l | head -n 5 ; echo 'Hello, World!' ; echo 'Goodbye, World!' ; echo 'The end.' && true || false";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Failed to parse command tree");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing chained commands should succeed");
}
