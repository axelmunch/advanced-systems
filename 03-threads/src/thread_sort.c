#include "thread_sort.h"

int tab[SIZE];
/**
 * @brief Sort an array using threads
 * @param tab Array to sort
 * @param size Size of the array
 * @return void
 *
 * This function will create a thread for each half of the array
 */

void thread_sort(int tan[])
{

}

/**
 * @brief Initialize the array with random values
 * @return void
 *
 * This function will initialize the array with random values
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
 * This function will print the array
 */
void print_array()
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", tab[i]);
    }
    printf("\n");
}
