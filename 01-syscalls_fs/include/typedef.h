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

/**
 * Enum to store the command mode
 */
typedef enum
{
    COPY_MODE,
    REVERSE_MODE,
    LIST_MODE,
    UNDEFINED_MODE
} command_mode;

/**
 * Struct to store file information
 */
typedef struct
{
    int fd;
    char buffer[BUFFER_SIZE];
    int index;
    int buffer_size_taken;
    bool write_mode; // Write mode. Else: reading only
} FICHIER;

#endif // TYPEDEF_H
