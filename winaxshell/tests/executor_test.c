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

Test(executor, null_node) 
{
    cr_assert_eq(execute_command_tree(NULL), EXIT_SUCCESS, "Executing a NULL node should succeed");
}

Test(executor, op_none) 
{
    node->args = (char *[]){"true", NULL};
    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "Command `true` should succeed");
}

Test(executor, op_and_success) 
{
    left->args = (char *[]){"true", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_AND;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`true && true` should succeed");
}

Test(executor, op_or_fail_left) 
{
    left->args = (char *[]){"false", NULL};
    right->args = (char *[]){"true", NULL};
    node->op_type = OP_OR;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`false || true` should succeed");
}

Test(executor, op_seq) 
{
    left->args = (char *[]){"echo", "hello", NULL};
    right->args = (char *[]){"echo", "world", NULL};
    node->op_type = OP_SEQ;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`echo hello ; echo world` should succeed");
}

Test(executor, op_pipe) 
{
    left->args = (char *[]){"echo", "hello", NULL};
    right->args = (char *[]){"grep", "hello", NULL};
    node->op_type = OP_PIPE;
    node->left = left;
    node->right = right;

    cr_assert_eq(execute_command_tree(node), EXIT_SUCCESS, "`echo hello | grep hello` should succeed");
}