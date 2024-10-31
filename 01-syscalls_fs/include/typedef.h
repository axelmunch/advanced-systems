#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * Struct to store binary parameters
 */
typedef struct
{
    char *input;
    char *output;
} binary_params_t;

typedef enum
{
    COPY_MODE,
    REVERSE_MODE,
    LIST_MODE,
    UNDEFINED_MODE
} command_mode;

#endif // TYPEDEF_H
