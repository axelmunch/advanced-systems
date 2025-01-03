#include "thread_sort.h"

int tab[SIZE];
pthread_mutex_t mutex;
int num_threads;

void initialize_array()
{
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        tab[i] = rand();
}

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

void *thread_arg_search(void *arg)
{
    int return_code;
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

    return_code = pthread_mutex_lock(&mutex);
    if (return_code != 0)
    {
        print_error("[ERROR] Mutex lock failed");
        exit(EXIT_FAILURE);
    }
    if (local_min < data->result->min)
        data->result->min = local_min;
    if (local_max > data->result->max)
        data->result->max = local_max;

    return_code = pthread_mutex_unlock(&mutex);
    if (return_code != 0)
    {
        print_error("[ERROR] Mutex unlock failed");
        exit(EXIT_FAILURE);
    }

    pthread_exit(NULL);
}

void get_min_max_threaded(min_max_t *result)
{
    int return_code;
    pthread_t threads[num_threads];
    thread_data_t thread_args[num_threads];
    int segment_size = SIZE / num_threads;


    return_code = pthread_mutex_init(&mutex, NULL);
    if (return_code != 0)
    {
        print_error("[ERROR] Mutex init failed");
        exit(EXIT_FAILURE);
    }
    result->min = INT_MAX;
    result->max = INT_MIN;

    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i].tab = tab;
        thread_args[i].start = i * segment_size;
        thread_args[i].end = (i == num_threads - 1) ? SIZE : (i + 1) * segment_size;
        thread_args[i].result = result;

        return_code = pthread_create(&threads[i], NULL, thread_arg_search, (void *)&thread_args[i]);
        if (return_code != 0)
        {
            print_error("[ERROR] Thread creation failed");
            pthread_mutex_destroy(&mutex);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        return_code = pthread_join(threads[i], NULL);
        if (return_code != 0)
        {
            print_error("[ERROR] Thread join failed");
            pthread_mutex_destroy(&mutex);
            exit(EXIT_FAILURE);
        }
    }

    return_code = pthread_mutex_destroy(&mutex);
    if (return_code != 0)
    {
        print_error("[ERROR] Mutex destroy failed");
        exit(EXIT_FAILURE);
    }
}

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

void print_results(min_max_t *result, double time_used, double time_unit)
{
    print_generic(STDOUT_FILENO, "Min: %d -- Max: %d\n", result->min, result->max);
    print_generic(STDOUT_FILENO, "Time used: %.4f %s\n", time_used, time_unit == TIME_MSEC ? "ms" : "sec");
}
