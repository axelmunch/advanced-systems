#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include <criterion/redirect.h>
#include "executor.h"
#include "parser.h"

command_node_t *node;
command_node_t *left;
command_node_t *right;

void setup(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();

    node = create_command_node();
    cr_assert_not_null(node, "Failed to create root node");
    node->op_type = OP_NONE;
    node->args = NULL;
    node->left = NULL;
    node->right = NULL;

    left = create_command_node();
    cr_assert_not_null(left, "Failed to create left node");
    left->op_type = OP_NONE;
    left->args = NULL;
    left->left = NULL;
    left->right = NULL;

    right = create_command_node();
    cr_assert_not_null(right, "Failed to create right node");
    right->op_type = OP_NONE;
    right->args = NULL;
    right->left = NULL;
    right->right = NULL;
}

TestSuite(executor, .init = setup);

Test(executor, test_null_node)
{
    int status = execute_command_tree(NULL);
    cr_assert_eq(status, EXIT_SUCCESS, "Executing a NULL node should succeed");
}

Test(executor, test_op_none)
{
    node->args = (char *[]){"true", NULL};
    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "Command `true` should succeed");
}

Test(executor, test_op_and_success)
{
    left->args = (char *[]){"true", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_AND;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`true && true` should succeed");
}

Test(executor, test_op_or_fail_left)
{
    left->args = (char *[]){"false", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_OR;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`false || true` should succeed");
}

Test(executor, test_op_seq)
{
    left->args = (char *[]){"true", NULL};
    right->args = (char *[]){"false", NULL};
    node->op_type = OP_SEQ;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_FAILURE, "`true ; false` should fail");
}

Test(executor, test_op_pipe)
{
    left->args = (char *[]){"ps", "-aux", NULL};
    right->args = (char *[]){"grep", "^root", NULL};
    node->op_type = OP_PIPE;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`ps -aux | grep ^root` should succeed");
}

Test(executor, test_op_bg)
{
    left->args = (char *[]){"sleep", "5", NULL};
    node->op_type = OP_BG;
    node->left = left;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`sleep 5 &` should succeed");
}

Test(executor, test_op_redir_out)
{
    left->args = (char *[]){"echo", "Hello, World!", NULL};
    right->args = (char *[]){"mocks/output.txt", NULL};
    node->op_type = OP_REDIR_OUT;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`echo Hello, World! > mocks/output.txt` should succeed");

    FILE *file = fopen("mocks/output.txt", "r");
    char *expected = "Hello, World!\n";
    char result[BUFFER_SIZE];
    fread(result, 1, sizeof(result), file);
    fclose(file);

    cr_assert_str_eq(result, expected, "Redirected output should be `Hello, World!`");
}

Test(executor, test_op_redir_in)
{
    left->args = (char *[]){"cat", NULL};
    right->args = (char *[]){"mocks/output.txt", NULL};
    node->op_type = OP_REDIR_IN;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`cat < mocks/output.txt` should succeed");
}

Test(executor, test_op_append)
{
    left->args = (char *[]){"echo", "Hello, World!", NULL};
    right->args = (char *[]){"mocks/output.txt", NULL};
    node->op_type = OP_APPEND;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`echo Hello, World! >> mocks/output.txt` should succeed");
}

Test(executor, test_op_heredoc)
{
    left->args = (char *[]){"cat", NULL};
    right->args = (char *[]){"Makefile", NULL};
    node->op_type = OP_HEREDOC;
    node->left = left;
    node->right = right;

    int status = execute_command_tree(node);
    cr_assert_eq(status, EXIT_SUCCESS, "`cat << Makefile` should succeed");
}
