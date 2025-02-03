#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/redirect.h>
#include <signal.h>
#include <sys/stat.h>
#include "utils.h"

void utils_setup(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

TestSuite(utils, .init = utils_setup);

Test(utils, test_free_if_needed_null)
{
    void *ptr = NULL;
    free_if_needed(ptr);
    cr_assert(1, "Should not crash with NULL pointer");
}

Test(utils, test_free_if_needed_malloc)
{
    void *ptr = malloc(sizeof(int));
    free_if_needed(ptr);
    cr_assert(1, "Should free malloc'd memory without crash");
}

Test(utils, test_free_if_needed_strdup)
{
    char *str = strdup("test string");
    free_if_needed(str);
    cr_assert(1, "Should free strdup'd string without crash");
}

Test(utils, test_free_if_needed_multiple)
{
    void *ptr1 = malloc(10);
    void *ptr2 = malloc(20);
    free_if_needed(ptr1);
    free_if_needed(ptr2);
    cr_assert(1, "Should handle multiple frees");
}

Test(utils, test_is_all_space)
{
    cr_assert(is_all_space("   \t\n"));
    cr_assert(!is_all_space("not space"));
    cr_assert(!is_all_space("  with space  "));
    cr_assert(is_all_space(""));
}

Test(utils, test_add_history_entry)
{
    setenv("HOME", "/tmp", 1);
    add_history_entry("test command");

    char path[256];
    snprintf(path, sizeof(path), "/tmp/%s", HISTORY_FILE);
    FILE *f = fopen(path, "r");
    cr_assert_not_null(f);

    char line[256];
    fgets(line, sizeof(line), f);
    cr_assert_str_eq(line, "test command\n");
    fclose(f);
}

Test(utils, test_add_history_entry_null)
{
    add_history_entry(NULL);
    add_history_entry("   ");
    cr_assert(1); // Should not crash
}

Test(utils, test_print_command_tree)
{
    command_node_t *node = malloc(sizeof(command_node_t));
    node->op_type = OP_PIPE;
    node->args = malloc(2 * sizeof(char *));
    node->args[0] = strdup("ls");
    node->args[1] = NULL;
    node->left = NULL;
    node->right = NULL;

    print_command_tree(node, 0);

    fflush(stderr);
    cr_assert_stderr_eq_str("Node: op_type=PIPE, args=[ls]\n");

    free_command_node(node);
}

Test(utils, test_safe_open_valid)
{
    char test_file[] = "/tmp/test_safe_open_XXXXXX";
    int tmp_fd = mkstemp(test_file);
    close(tmp_fd);

    int fd = safe_open(test_file, O_RDONLY, 0644);
    cr_assert(fd >= 0, "Failed to open valid file");
    close(fd);
    unlink(test_file);
}

Test(utils, test_safe_open_invalid_path, .exit_code = EXIT_FAILURE)
{
    safe_open("/nonexistent/path", O_RDONLY, 0644);
    cr_assert_fail("Should not reach this point");
}

Test(utils, test_safe_open_permission_denied, .exit_code = EXIT_FAILURE)
{
    char test_file[] = "/tmp/test_safe_open_perm_XXXXXX";
    int tmp_fd = mkstemp(test_file);
    close(tmp_fd);
    chmod(test_file, 0000);

    safe_open(test_file, O_RDONLY, 0644);
    cr_assert_fail("Should not reach this point");

    chmod(test_file, 0644);
    unlink(test_file);
}

Test(utils, test_set_env_var_errors)
{
    cr_assert_eq(set_env_var("invalid"), -1);
    cr_assert_eq(set_env_var("=noname"), -1);
}

Test(utils, test_verbose_mode)
{
    cr_assert_eq(get_verbose_mode(), false, "Default verbose mode should be false");
    set_verbose_mode(true);
    cr_assert_eq(get_verbose_mode(), true, "Verbose mode should be true after setting");
    set_verbose_mode(false);
    cr_assert_eq(get_verbose_mode(), false, "Verbose mode should be false after resetting");
}

Test(utils, test_print_verbose)
{
    set_verbose_mode(true);
    print("Test message\n");
    cr_assert_stdout_eq_str("Test message\n");
}

Test(utils, test_print_non_verbose)
{
    set_verbose_mode(false);
    print("Test message\n");
    cr_assert_stdout_eq_str("");
}

Test(utils, test_get_operator_str)
{
    cr_assert_str_eq(get_operator_str(OP_NONE), "NONE");
    cr_assert_str_eq(get_operator_str(OP_PIPE), "PIPE");
    cr_assert_str_eq(get_operator_str(OP_SEQ), "SEQ");
    cr_assert_str_eq(get_operator_str(OP_AND), "AND");
    cr_assert_str_eq(get_operator_str(OP_OR), "OR");
    cr_assert_str_eq(get_operator_str(OP_BG), "BG");
    cr_assert_str_eq(get_operator_str(OP_REDIR_OUT), "REDIR_OUT");
    cr_assert_str_eq(get_operator_str(OP_REDIR_IN), "REDIR_IN");
    cr_assert_str_eq(get_operator_str(OP_APPEND), "APPEND");
    cr_assert_str_eq(get_operator_str(OP_HEREDOC), "HEREDOC");
    cr_assert_str_eq(get_operator_str((operator_t)100), "UNKNOWN");
}