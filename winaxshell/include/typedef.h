#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * @brief Operator types
 * @param OP_NONE (none) No operator
 * @param OP_PIPE (|) Pipe operator
 * @param OP_SEQ (;) Sequential operator
 * @param OP_AND (&&) And operator
 * @param OP_OR (||) Or operator
 * @param OP_BG (&) Background operator
 */
typedef enum
{
    OP_NONE,
    OP_PIPE,
    OP_SEQ,
    OP_AND,
    OP_OR,
    OP_BG
} operator_t;

/**
 * @brief Command node structure
 * @param args Command arguments
 * @param op_type Operator following this command
 * @param left Left child (current command)
 * @param right Right child (next command)
 * @see operator_t
 */
typedef struct command_node_t
{
    char **args;
    operator_t op_type;
    struct command_node_t *left;
    struct command_node_t *right;
} command_node_t;

/**
 * @brief Command tree structure
 * @param root Root node of the command tree
 */
typedef struct
{
    command_node_t *root;
} command_tree_t;

#endif // TYPEDEF_H
