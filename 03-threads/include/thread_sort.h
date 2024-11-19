#ifndef THREAD_SORT_H
#define THREAD_SORT_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

void thread_sort(int tab[]);
void initialize_array();

extern int tab[SIZE];

#endif // THREAD_SORT_H
