#include "thread_sort.h"

int tab[SIZE];
int min, max;

/**
 * @brief Initialize the array with random values
 * @return void
 *
 */
void initialize_array()
{
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
    {
        tab[i] = rand();
    }
}

/**
 * @brief Print the array
 * @return void
 *
 */
void print_array()
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

/**
 * @brief get the minimum value of an array
 * @param tab Array to sort
 * @return int
 *
 */
int get_min(int tab[])
{
    int min = tab[0];
    for (int i = 0; i < SIZE; i++)
    {
        if (tab[i] < min)
        {
            min = tab[i];
        }
    }
    return min;
}

/**
 * @brief get the maximum value of an array
 * @param tab Array to sort
 * @return int
 *
 */
int get_max(int tab[])
{
    int max = tab[0];
    for (int i = 0; i < SIZE; i++)
    {
        if (tab[i] > max)
        {
            max = tab[i];
        }
    }
    return max;
}

/**
 * @brief find the min and max values of an array
 * @param tab Array to find min and max
 * @param min Pointer to store the min value
 * @param max Pointer to store the max value
 * @return void
 *
 */
void find_min_max_sequential(int tab[], int *min, int *max)
{
    struct timeval start, end;
    long seconds, useconds;
    double mtime;

    gettimeofday(&start, NULL);
    *min = get_min(tab);
    *max = get_max(tab);
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    print_results(mtime);
}

/**
 * @brief print the results and the time taken
 * @return void
 *
 */
void print_results(double time_taken)
{
    print_generic(STDOUT_FILENO, "==== RESULTS ====\n");
    print_generic(STDOUT_FILENO, "Min: %d -- Max: %d\n", min, max);
    print_generic(STDOUT_FILENO, "Time taken: %f ms\n", time_taken);
}
