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
    cr_assert_eq(status, EXIT_SUCCESS, "Executing chained pipes should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char *expected_output = "1\n";
    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '%s', got '%s'", expected_output, buffer);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_chained_commands)
{
    char *input = "ls -l | head -n 5 ; echo 'Hello, World!' ; echo 'Goodbye, World!' ; echo 'The end.' && true || false";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Failed to parse command tree");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing chained commands should succeed, got %d", status);
}

Test(integration, test_chained_redir)
{
    char *input = "cat < Makefile > mocks/output.txt && echo 'Redirect succeeded' || echo 'Failed to redirect'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Failed to parse command tree");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing chained redirections should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char *expected_output = "Redirect succeeded\n";
    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '%s', got '%s'", expected_output, buffer);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_command_with_env)
{
    char *input = "a='Hello, World!' b='Goodbye, World!' c='The end,;:!@#$%^&*()' ; echo $a ; echo $b ; echo $c && echo 'All environment variables set' || echo 'Failed to set environment variables'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Failed to parse command tree");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing command with environment variable should succeed, got %d", status);
}

Test(integration, test_env_in_single_quote)
{
    char *input = "a=\"Hello\" b=\"World\" ; echo '$a $b'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing command with environment variable in quotes should succeed");

    FILE *output = cr_get_redirected_stdout();
    char *expected_output = "Hello World\n";

    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '%s', got '%s'", expected_output, buffer);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_env_in_double_quotes)
{
    char *input = "a=\"Hello\" b=\"World\" ; echo \"$a $b\"";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing command with environment variable in quotes should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char *expected_output = "Hello World\n";

    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '%s', got '%s'", expected_output, buffer);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_final_boss_env)
{
    char *input = "a=\'totooooooo\' ; b=titiiiiii!!!!!!!!!!! ; c=tutututututututuu??????????? ; d=\"OK, stop maintenant c'est bon. That's all Folks!\" ; echo \"$a $b $c       $d\" && echo 'All environment variables set'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing final boss env command should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char *expected_output = "totooooooo titiiiiii!!!!!!!!!!! tutututututututuu???????????       OK, stop maintenant c'est bon. That's all Folks!\n";

    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), output);
    cr_assert_str_eq(buffer, expected_output, "Output should be '%s', got '%s'", expected_output, buffer);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_pipe_and_command)
{
    char *input = "ls -l | grep Makefile && ps aux | grep '^root'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing pipe and command should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), output) != NULL)
    {
        if (strstr(buffer, "Makefile") != NULL)
        {
            break;
        }
    }
    cr_assert_not_null(strstr(buffer, "Makefile"), "Output should contain 'Makefile'");

    while (fgets(buffer, sizeof(buffer), output) != NULL)
    {
        if (strstr(buffer, "root") != NULL)
        {
            break;
        }
    }
    cr_assert_not_null(strstr(buffer, "root"), "Output should contain 'root'");

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_create)
{
    char *input = "alias a='ls -l' ; a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "Alias should not yet be defined, got %d", status);

    input = "a";
    tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing alias command should succeed, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_too_many_args)
{
    char *input = "alias a a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "Alias command with too many arguments should fail, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_unalias)
{
    char *input = "alias a='ls -l' ; alias b='jarjar' ; unalias a ; unalias a ; a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "Alias command unaliased should fail, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_unalias_too_many_args)
{
    char *input = "unalias a a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "Unalias command with too many arguments should fail, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_list)
{
    char *input = "alias a='true' ; alias b='true' ; alias c='true' ; alias";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing alias command should succeed, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_alias_redefine)
{
    char *input = "alias a='exit 1' ; alias a='exit 0'";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing alias command should succeed, got %d", status);

    input = "a";
    tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Alias should be replaced, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_ls_custom_with_param)
{
    char *input = "ls_custom .";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "ls_custom command with path should succeed, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_ls_custom_too_many_args)
{
    char *input = "ls_custom a a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "ls_custom command with too many arguments should fail, got %d", status);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(integration, test_kill_background_command)
{
    char *input = "sleep 1000 &";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing background command should succeed, got %d", status);

    FILE *output = cr_get_redirected_stdout();
    char buffer[256];
    fgets(buffer, sizeof(buffer), output);

    int pid;
    cr_assert_eq(sscanf(buffer, "[%d]", &pid), 1, "Failed to get background process PID");
    cr_assert_gt(pid, 0, "Invalid PID value: %d", pid);

    cr_assert_eq(kill(pid, SIGTERM), 0, "Failed to send SIGTERM to process %d", pid);

    usleep(10000);

    char termination_buffer[256];
    fgets(termination_buffer, sizeof(termination_buffer), output);
    char expected_msg[256];
    snprintf(expected_msg, sizeof(expected_msg), "[%d] Process killed by signal %d", pid, SIGTERM);
    cr_assert_not_null(strstr(termination_buffer, expected_msg), "Expected termination message not found: %s", expected_msg);

    free_command_node(tree->root);
    free_if_needed(tree);
}
