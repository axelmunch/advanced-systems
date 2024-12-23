#include "thread_sort.h"

int tab[SIZE];
pthread_mutex_t mutex;
int num_threads;

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
void get_min_max(min_max_t *result)
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
 * @param min_max_t struct to store the min and max values
 * @param time_unit Time unit to display
 * @return void
 */
void sequential_search(min_max_t *result, double time_unit)
{
    struct timeval start, end;
    long seconds, useconds;
    double time_used;

    gettimeofday(&start, NULL);
    get_min_max(result);
    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    time_used = ((seconds)*time_unit + useconds / time_unit);

    print_generic(STDOUT_FILENO, ">>> Sequential search :\n");
    print_results(result, time_used, time_unit);
}

/**
 * @brief find the min and max values of an array using threads
 * @param min_max_t struct to store the min and max values
 * @param time_unit Time unit to display
 * @param thread Number of threads to use
 * @return void
 */
void threaded_search(min_max_t *result, double time_unit, int thread)
{
    struct timeval start, end;
    long seconds, useconds;
    double time_used;

    gettimeofday(&start, NULL);
    get_min_max_threaded(result);
    gettimeofday(&end, NULL);

    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    time_used = ((seconds)*time_unit + useconds / time_unit);

    print_generic(STDOUT_FILENO, ">>> Thread search :\n");
    print_results(result, time_used, time_unit);
}

/**
 * @brief print the results and the time taken
 * @return void
 */
void print_results(min_max_t *result, double time_used, double time_unit)
{
    print_generic(STDOUT_FILENO, "Min: %d -- Max: %d\n", result->min, result->max);
    print_generic(STDOUT_FILENO, "Time used: %.4f %s\n", time_used, time_unit == TIME_MSEC ? "ms" : "sec");
}

/**
 * @brief get the min and max values of an array using threads
 * @arg void* arg pointer to the thread data
 */
void *thread_arg_search(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    int local_min = data->tab[data->start];
    int local_max = data->tab[data->start];

    for (int i = data->start; i < data->end; i++)
    {
        if (data->tab[i] < local_min)
            local_min = data->tab[i];
        if (data->tab[i] > local_max)
            local_max = data->tab[i];
    }

    pthread_mutex_lock(&mutex);
    if (local_min < data->result->min)
        data->result->min = local_min;
    if (local_max > data->result->max)
        data->result->max = local_max;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

/**
 * @brief find_min_max_threaded
 * @return void
 */
void get_min_max_threaded(min_max_t *result)
{
    pthread_t threads[num_threads];
    thread_data_t thread_args[num_threads];
    int segment_size = SIZE / num_threads;

    pthread_mutex_init(&mutex, NULL);
    result->min = INT_MAX;
    result->max = INT_MIN;

    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i].tab = tab;
        thread_args[i].start = i * segment_size;
        thread_args[i].end = (i == num_threads - 1) ? SIZE : (i + 1) * segment_size;
        thread_args[i].result = result;

        pthread_create(&threads[i], NULL, thread_arg_search, (void *)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
}
