#include <criterion/criterion.h>
#include "executor.h"

Test(executor, null_node) {
    cr_assert_eq(execute_command_tree(NULL), EXIT_SUCCESS);
}

Test(executor, op_none) {
    command_node_t node = {
        .op_type = OP_NONE,
        .args = (char *[]){"echo", "Test", NULL},
        .left = NULL,
        .right = NULL
    };
    cr_assert_eq(execute_command_tree(&node), EXIT_SUCCESS);
}

Test(executor, op_and_success) {
    command_node_t left = {
        .op_type = OP_NONE,
        .args = (char *[]){"true", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t right = {
        .op_type = OP_NONE,
        .args = (char *[]){"true", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t node = {
        .op_type = OP_AND,
        .left = &left,
        .right = &right
    };
    cr_assert_eq(execute_command_tree(&node), EXIT_SUCCESS);
}

Test(executor, op_or_fail_left) {
    command_node_t left = {
        .op_type = OP_NONE,
        .args = (char *[]){"false", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t right = {
        .op_type = OP_NONE,
        .args = (char *[]){"true", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t node = {
        .op_type = OP_OR,
        .left = &left,
        .right = &right
    };
    cr_assert_eq(execute_command_tree(&node), EXIT_SUCCESS);
}

Test(executor, op_seq) {
    command_node_t left = {
        .op_type = OP_NONE,
        .args = (char *[]){"true", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t right = {
        .op_type = OP_NONE,
        .args = (char *[]){"true", NULL},
        .left = NULL,
        .right = NULL
    };
    command_node_t node = {
        .op_type = OP_SEQ,
        .left = &left,
        .right = &right
    };
    cr_assert_eq(execute_command_tree(&node), EXIT_SUCCESS);
}