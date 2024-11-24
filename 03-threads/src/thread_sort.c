#include "thread_sort.h"

int tab[SIZE];


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

/**
 * @brief Sort an array using threads
 * @param tab Array to sort
 * @return void
 *
 * This function will create a thread for each half of the array
 */

void thread_sort(int tab[])
{

}

/**
 * @brief get the minimum value of an array
 * @param tab Array to sort
 * @return int
 *
 */
int get_min(int tab[])
{
    int min = tab[0];
    for (int i = 0; i < SIZE; i++)
    {
        if (tab[i] < min)
        {
            min = tab[i];
        }
    }
    return min;
}

/**
 * @brief get the maximum value of an array
 * @param tab Array to sort
 * @return int
 *
 */
int get_max(int tab[])
{
    int max = tab[0];
    for (int i = 0; i < SIZE; i++)
    {
        if (tab[i] > max)
        {
            max = tab[i];
        }
    }
    return max;
}
