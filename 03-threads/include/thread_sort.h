#ifndef THREAD_SORT_H
#define THREAD_SORT_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>

#include "constants.h"
#include "print.h"
#include "typedef.h"

extern int tab[SIZE];
extern int num_threads;
extern pthread_mutex_t mutex;

/**
 * @brief Initialize the array with random values
 * @return void
 */
void initialize_array();

/**
 * @brief Get the minimum and maximum values of an array
 * @param tab Array to search
 * @param result Pointer to store the min and max values
 * @return void
 */
void get_min_max(min_max_t *result);

/**
 * @brief find the min and max values of an array. Interface function to call in main
 * @param min_max_t struct to store the min and max values
 * @param time_unit Time unit to display
 * @return void
 */
void sequential_search(min_max_t *result, double time_unit);

/**
 * @brief Thread argument function to search for the min and max values with mutexes locks
 * @param void* arg pointer to the thread data
 * @return void
 */
void *thread_arg_search(void *arg);

/**
 * @brief Get the min and max values of an array using multiple threads and join them
 * @param min_max_t struct to store the min and max values
 * @return void
 */
void get_min_max_threaded(min_max_t *result);

/**
 * @brief find the min and max values of an array using threads. Interface function to call in main
 * @param min_max_t struct to store the min and max values
 * @param time_unit Time unit to display
 * @param thread Number of threads to use
 * @return void
 */
void threaded_search(min_max_t *result, double time_unit, int thread);

/**
 * @brief Print the results and the time taken
 * @param min_max_t struct to store the min and max values
 * @param time_used Time taken to search
 * @param time_unit Time unit to display
 * @return void
 */
void print_results(min_max_t *result, double time_used, double time_unit);

#endif // THREAD_SORT_H
