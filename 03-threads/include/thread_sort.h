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

void initialize_array();
void get_min_max(min_max_t *result);
void sequential_search(min_max_t *result, double time_unit);
void *thread_arg_search(void *arg);
void get_min_max_threaded(min_max_t *result);
void threaded_search(min_max_t *result, double time_unit, int thread);
void print_results(min_max_t *result, double time_used, double time_unit);

extern int tab[SIZE];
extern int num_threads;
extern pthread_mutex_t mutex;

#endif // THREAD_SORT_H
