#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * @brief Command mode of the program
 */
typedef enum
{
    FORK_MODE,
    REDIRECT_MODE,
    PIPE_MODE,
    UNDEFINED_MODE
} command_mode;

#endif // TYPEDEF_H