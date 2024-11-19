#include "thread_sort.h"

/**
 * @brief Sort an array using threads
 * @param tab Array to sort
 * @param size Size of the array
 * @return void
 */

void thread_sort(int tan[])
{

}

/**
 * @brief Initialize the array with random values
 * @return void
 */
void initialize_array()
{
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
    {
        tab[i] = rand();
    }
}
