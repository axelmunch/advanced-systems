#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * @brief Operator types
 * @param OP_NONE No operator : default
 * @param OP_PIPE Pipe operator : redirect output of the left command to the input of the right command (|)
 * @param OP_SEQ Sequential operator : execute the right command after the left command (;)
 * @param OP_AND And operator : execute the right command only if the left command is successful (&&)
 * @param OP_OR Or operator : execute the right command only if the left command fails (||)
 * @param OP_BG Background operator : execute the command in the background (&)
 * @param OP_REDIR_OUT Redirect output operator : redirect output to a file (>)
 * @param OP_REDIR_IN Redirect input operator : redirect input from a file (<)
 * @param OP_APPEND Append output operator : append output to a file (>>)
 * @param OP_HEREDOC Heredoc operator : redirect input from a heredoc (<<)
 */
typedef enum
{
    OP_NONE,
    OP_PIPE,
    OP_SEQ,
    OP_AND,
    OP_OR,
    OP_BG,
    OP_REDIR_OUT,
    OP_REDIR_IN,
    OP_APPEND,
    OP_HEREDOC
} operator_t;

/**
 * @brief Command node structure in a command tree
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
 * @brief Command tree structure to represent a command line
 * @param root Root node of the command tree
 */
typedef struct
{
    command_node_t *root;
} command_tree_t;

#endif // TYPEDEF_H