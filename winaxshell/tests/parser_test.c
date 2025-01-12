#include <criterion/criterion.h>
#include <string.h>
#include "parser.h"

Test(parser, test_create_command_node)
{
    command_node_t *node = create_command_node();
    cr_assert_not_null(node, "Node should not be NULL");
    cr_assert_not_null(node->args, "Node args should not be NULL");
    free_command_node(node);
}

Test(parser, test_create_command_tree)
{
    command_tree_t *tree = create_command_tree();
    cr_assert_not_null(tree, "Tree should not be NULL");
    cr_assert_not_null(tree->root, "Tree root should not be NULL");
    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(parser, test_handle_operator)
{
    command_tree_t *tree = create_command_tree();
    cr_assert_not_null(tree, "Tree should not be NULL");
    command_node_t *op_node = handle_operator(tree, OP_PIPE);
    cr_assert_not_null(op_node, "Operator node should not be NULL");
    cr_assert_eq(op_node->op_type, OP_PIPE, "Operator type should be OP_PIPE");
    free_command_node(op_node);
    free_if_needed(tree);
}

Test(parser, test_handle_argument)
{
    command_node_t *node = create_command_node();
    cr_assert_not_null(node, "Node should not be NULL");
    int result = handle_argument(node, "arg1", 0);
    cr_assert_eq(result, 1, "handle_argument should succeed");
    cr_assert_str_eq(node->args[0], "arg1", "Argument should match input");
    free_command_node(node);
}

Test(parser, test_parse_command)
{
    command_tree_t *tree = parse_command("ls -l | grep txt");
    cr_assert_not_null(tree, "Tree should not be NULL");
    cr_assert_not_null(tree->root->left, "Left node should not be NULL after parsing operator");
    cr_assert_str_eq(tree->root->left->args[0], "ls", "First argument in left node should be 'ls'");
    cr_assert_str_eq(tree->root->left->args[1], "-l", "Second argument in left node should be '-l'");
    cr_assert_not_null(tree->root->right, "Right node should not be NULL after parsing operator");
    cr_assert_str_eq(tree->root->right->args[0], "grep", "First argument in right node should be 'grep'");
    cr_assert_str_eq(tree->root->right->args[1], "txt", "Second argument in right node should be 'txt'");
    free_command_node(tree->root);
    free_if_needed(tree);
}

Test(parser, test_get_operator_type)
{
    cr_assert_eq(get_operator_type("|"), OP_PIPE, "should return OP_PIPE");
    cr_assert_eq(get_operator_type("&&"), OP_AND, "should return OP_AND");
    cr_assert_eq(get_operator_type("||"), OP_OR, "should return OP_OR");
    cr_assert_eq(get_operator_type(";"), OP_SEQ, "should return OP_SEQ");
    cr_assert_eq(get_operator_type("&"), OP_BG, "should return OP_BG");
    cr_assert_eq(get_operator_type(NULL), OP_NONE, "NULL should return OP_NONE");
    cr_assert_eq(get_operator_type("@"), OP_NONE, "Unknown operator should return OP_NONE");
}

Test(parser, test_parse_double_quotes)
{
    command_tree_t *tree = parse_command("echo \"Hello, World!\"");
    cr_assert_not_null(tree, "Tree should not be NULL");
    cr_assert_str_eq(tree->root->args[0], "echo", "First argument should be 'echo', got '%s'", tree->root->args[0]);
    cr_assert_str_eq(tree->root->args[1], "Hello, World!", "Second argument should be 'Hello, World!', got '%s'", tree->root->args[1]);
    free_command_node(tree->root);
    free_if_needed(tree);
}