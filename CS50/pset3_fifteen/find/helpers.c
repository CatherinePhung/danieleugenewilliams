/**************************************************************************** 
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 ***************************************************************************/
       
#include <cs50.h>
#include <math.h>

#include "helpers.h"


/*
 * Returns true if value is in array of n values, else false.
 */

bool 
search(int value, int values[], int n)
{
    // re-implemented as binary search
    while (n > 0) 
    {
        // cut list in half
        int mid = round(n/2);
        
        if(values[mid] == value)
            // if number found, return true
            return true;
        else if (values[mid] > value)
        {
            // only consider left half of list
            for (int i = mid; i > 0; i--)
            {
                if(values[i] == value)
                    return true;
            }
        }
            
        else if (values[mid] < value)
        {
            // only consider right half of list
            for (int i = mid; i < n; i++)
            {
                if(values[i] == value)
                    return true;
            }
        }
    // decrement list
    n--;
    }
    return false;
}


/*
 * Sorts array of n values.
 */

void 
sort(int values[], int n)
{
    // selection sort
    int min = 0;
    int temp = 0;
    for (int i = 0; i < n - 1; i++)
    {
        min = i;
        // traverse list
        for (int j = i + 1; j < n; j++)
        {
            // find minimum value in the list
            if (values[j] < values[min])
            {
                min = j;
            }
            
            // if value to the right is less than the value to left, swap
            if (values[min] < values[i])
            {
                // swap array[min] and array[i]
                temp = values[min];
                values[min] = values[i];
                values[i] = temp;
            }
        }
    }
}
