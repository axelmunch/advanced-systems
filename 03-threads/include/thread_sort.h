#ifndef THREAD_SORT_H
#define THREAD_SORT_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "constants.h"
#include "print.h"

void initialize_array();
void print_array();
int get_min(int tab[]);
int get_max(int tab[]);
void find_min_max_sequential(int tab[], int *min, int *max);
void print_results();

extern int tab[SIZE];
extern int min, max;

#endif // THREAD_SORT_H
