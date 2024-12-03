#ifndef THREAD_SORT_H
#define THREAD_SORT_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "constants.h"
#include "print.h"
#include "typedef.h"

void initialize_array();
void print_array();
void get_min_max(int tab[], min_max_t *result);
void get_min_max_threaded(int tab[], min_max_t *result, int thread, pthread_t threads[]);
void sequential_search(int tab[], min_max_t *result, double time_unit);
void threaded_search(int tab[], min_max_t *result, double time_unit, int thread);
void print_results(min_max_t *result, double time_used, double time_unit);

extern int tab[SIZE];

#endif // THREAD_SORT_H
