#include "thread_sort.h"

int tab[SIZE];

/**
 * @brief Initialize the array with random values
 * @return void
 */
void initialize_array()
{
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        tab[i] = rand();
}

/**
 * @brief Print the array
 * @return void
 */
void print_array()
{
    for (int i = 0; i < SIZE; i++)
        printf("%d ", tab[i]);

    printf("\n");
}

/**
 * @brief get the minimum and maximum values of an array
 * @param tab Array to search
 * @param result Pointer to store the min and max values
 * @return void
 */
void get_min_max(int tab[], min_max_t *result)
{
    result->min = tab[0];
    result->max = tab[0];
    for (int i = 1; i < SIZE; i++)
    {
        if (tab[i] < result->min)
            result->min = tab[i];
        if (tab[i] > result->max)
            result->max = tab[i];
    }
}

/**
 * @brief find the min and max values of an array
 * @param tab Array to find min and max
 * @param min Pointer to store the min value
 * @param max Pointer to store the max value
 * @return void
 */
void sequential_search(int tab[], min_max_t *result, double time_unit)
{
    struct timeval start, end;
    long seconds, useconds;
    double time_used;

    gettimeofday(&start, NULL);
    get_min_max(tab, result);
    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    time_used = ((seconds)*time_unit + useconds / time_unit);

    print_results(result, time_used, time_unit);
}

/**
 * @brief find the min and max values of an array using threads
 * @param tab Array to find min and max
 * @param min Pointer to store the min value
 * @param max Pointer to store the max value
 * @param time_unit Time unit to display
 * @param thread Number of threads to use
 * @return void
 */
void threaded_search(int tab[], min_max_t *result, double time_unit, int thread)
{
    struct timeval start, end;
    long seconds, useconds;
    double time_used;
    pthread_t threads[thread];

    gettimeofday(&start, NULL);
    get_min_max_threaded(tab, result, thread, threads);
    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    time_used = ((seconds)*time_unit + useconds / time_unit);

    print_results(result, time_used, time_unit);
}

/**
 * @brief print the results and the time taken
 * @return void
 */
void print_results(min_max_t* result, double time_used, double time_unit)
{
    print_generic(STDOUT_FILENO, "==== RESULTS ====\n");
    print_generic(STDOUT_FILENO, "Min: %d -- Max: %d\n", result->min, result->max);
    print_generic(STDOUT_FILENO, "Time used: %.4f %s\n", time_used, time_unit == TIME_MSEC ? "ms" : "sec");
}

/**
 * @brief get the min and max values of an array using threads
 * @param tab array to find min and max
 * @param result pointer to store the min and max values
 * @param thread number of threads to use
 * @param threads array of threads
 */
void get_min_max_threaded(int tab[], min_max_t* result, int thread, pthread_t threads[])
{

}
