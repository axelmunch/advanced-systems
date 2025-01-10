#include <criterion/criterion.h>
#include <criterion/hooks.h>
#include "executor.h"
#include "parser.h"

command_node_t* node;
command_node_t* left;
command_node_t* right;

void setup(void) 
{
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
    cr_assert_eq(execute_command_tree(NULL), EXIT_SUCCESS, "Executing a NULL node should succeed");
}

Test(executor, test_op_none) 
{
    node->args = (char *[]){"true", NULL};
    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "Command `true` should succeed");
}

Test(executor, test_op_and_success) 
{
    left->args = (char *[]){"true", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_AND;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`true && true` should succeed");
}

Test(executor, test_op_or_fail_left) 
{
    left->args = (char *[]){"false", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_OR;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`false || true` should succeed");
}

Test(executor, test_op_seq) 
{
    left->args = (char *[]){"echo", "foo", NULL};
    right->args = (char *[]){"echo", "bar", NULL};
    node->op_type = OP_SEQ;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`echo foo ; echo bar` should succeed");
}

Test(executor, test_op_pipe) 
{
    left->args = (char *[]){"echo", "Hello,", "foo", NULL};
    right->args = (char *[]){"grep", "foo", NULL};
    node->op_type = OP_PIPE;
    node->left = left;
    node->right = right;
    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`echo Hello, foo | grep foo` should succeed");
}

Test(executor, test_op_bg) 
{
    left->args = (char *[]){"sleep", "5", NULL};
    node->op_type = OP_BG;
    node->left = left;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`sleep 5 &` should succeed");
}

Test(executor, test_command_chain)
{
    char *input = "ls -l | head -n 5 | wc -l ; echo 'foo, bar, bazz' && false || echo 'Thats all folks!'";
    command_tree_t *tree = parse_command(input);

    cr_assert_not_null(tree, "Failed to parse command tree");
    cr_assert_eq(execute_command_tree(tree->root), EXIT_SUCCESS, "Executing command chain should succeed");

    free_command_node(tree->root);
    free_if_needed(tree);
}