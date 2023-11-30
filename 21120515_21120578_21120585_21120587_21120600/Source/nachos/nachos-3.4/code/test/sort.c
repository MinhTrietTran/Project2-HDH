/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int A[100]; /* size of physical memory; with code, we'll run out of space!*/

int main()
{
    int i, j, tmp;

    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < 100; i++)
        A[i] = 100 - i;

    /* then sort! */
    for (i = 0; i < 98; i++)
        for (j = 0; j < (98 - i); j++)
            if (A[j] > A[j + 1])
            { /* out of order -> need to swap ! */
                tmp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = tmp;
            }
    Exit(A[99]); /* exit with the value of the last element after sorting */
}
