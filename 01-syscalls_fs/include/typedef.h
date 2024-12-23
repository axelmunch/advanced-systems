#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * @brief typedef struct to store binary parameters
 */
typedef struct
{
    char *input;
    char *output;
} binary_params_t;

/**
 * @brief enum to store the command mode
 */
typedef enum
{
    COPY_MODE,
    REVERSE_MODE,
    LIST_MODE,
    BUFFERED_MODE,
    UNDEFINED_MODE
} command_mode;

/**
 * @brief typedef struct to store file information
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
