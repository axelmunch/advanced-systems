#ifndef TYPEDEF_H
#define TYPEDEF_H

typedef enum
{
    OP_NONE, // No operator
    OP_PIPE, // |
    OP_SEQ,  // ;
    OP_AND,  // &&
    OP_OR,   // ||
    OP_BG    // &
} operator_t;

typedef struct command_node_t
{
    char **args;                  // Command arguments
    operator_t op_type;           // Operator following this command
    struct command_node_t *left;  // Left child (current command)
    struct command_node_t *right; // Right child (next command)
} command_node_t;

typedef struct
{
    command_node_t *root; // Root node of the command tree
} command_tree_t;

#endif // TYPEDEF_H
