#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/redirect.h>
#include "executor.h"
#include "parser.h"

void custom_command_setup(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

TestSuite(custom_command, .init = custom_command_setup);

Test(custom_command, test_cd)
{
    char *input = "cd /home";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing cd command should succeed");

    char cwd[MAX_PATH_LENGTH];
    char *expected = "/home";
    char *result = getcwd(cwd, sizeof(cwd));
    cr_assert_str_eq(expected, result, "Current working directory should be %s, got %s", expected, result);
}

Test(custom_command, test_cd_home)
{
    char *input = "cd";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing cd command should succeed");

    char cwd[MAX_PATH_LENGTH];
    char *expected = getenv("HOME");
    char *result = getcwd(cwd, sizeof(cwd));
    cr_assert_str_eq(expected, result, "Current working directory should be %s, got %s", expected, result);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_cd_tilde)
{
    char *input = "cd ~";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing cd command should succeed");

    char cwd[MAX_PATH_LENGTH];
    char *expected = getenv("HOME");
    char *result = getcwd(cwd, sizeof(cwd));
    cr_assert_str_eq(expected, result, "Current working directory should be %s, got %s", expected, result);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_cd_nonexistent)
{
    char *input = "cd /nonexistent";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_FAILURE, "Executing cd command should fail");

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_pwd)
{
    char *input = "pwd";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing pwd command should succeed");

    FILE *output = cr_get_redirected_stdout();

    char cwd[MAX_PATH_LENGTH];
    char *expected = getcwd(cwd, sizeof(cwd));
    strcat(expected, "\n");
    char result[BUFFER_SIZE];
    fgets(result, sizeof(result), output);

    cr_assert_str_eq(result, expected, "Output should print working directory '%s', got '%s'", expected, result);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_echo)
{
    char *input = "echo foo bar bazzzz!";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing echo command should succeed");

    FILE *output = cr_get_redirected_stdout();
    char *expected = "foo bar bazzzz!\n";
    char result[BUFFER_SIZE];
    fgets(result, sizeof(result), output);
    cr_assert_str_eq(result, expected, "Output should print '%s', got '%s'", expected, result);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_echo_env)
{
    char *input = "a='Hello, World!' ; echo $a";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing echo command with environment variable should succeed");

    FILE *output = cr_get_redirected_stdout();
    char *expected = "Hello, World!\n";
    char result[BUFFER_SIZE];
    fgets(result, sizeof(result), output);
    cr_assert_str_eq(result, expected, "Output should print '%s', got '%s'", expected, result);

    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(custom_command, test_ls_custom)
{
    char *input = "ls_custom | grep Makefile";
    command_tree_t *tree = parse_command(input);
    cr_assert_not_null(tree, "Tree should not be NULL");

    int status = execute_command_tree(tree->root);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing ls_custom with pipe command should succeed");

    free_command_node(tree->root);
    free_if_needed(tree);
}
