#ifndef TYPEDEF_H
#define TYPEDEF_H

/**
 * @brief typedef for storing min and max values
 */
typedef struct
{
    int min;
    int max;
} min_max_t;

/**
 * @brief typedef for storing thread data
 */
typedef struct
{
    int *tab;
    int start;
    int end;
    min_max_t *result;
} thread_data_t;

#endif // TYPEDEF_H
