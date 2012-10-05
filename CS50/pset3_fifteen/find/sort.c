#include <cs50.h>
#include <stdio.h>
#include <math.h>

void selection_sort(int values[], int n);

void bubble_sort(int values[], int n);

bool search(int value, int array[], int n);
/*
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
14, 15, 17, 18, 19, 22, 23, 25, 31, 45, 99
*/

int
main(int argc, char* argv[])
{
    int num[25] = {7,9,1,6,31,12,8,5,13,25,18,45,2,15,22,0,4,3,99,14,11,23,17,19,10};
    
    selection_sort(num, 25);
    for(int i =0; i < 25; i++)
    {
        printf("%d, ", num[i]);
    }
    
    int key = atoi(argv[1]);
    printf("%d\n", search(key, num, 25));
    

}


// bubble sort: implement an O(n^2) sort
void
bubble_sort(int values[], int n)
{    
    int swapped = 1;
    int temp = 0;
    // while elements have been swapped
    while(swapped == 1)
    {
        swapped = 0;
        // loop thru each element
        for(int i = 0; i < n - 1; i++)
        {
            if (values[i] > values[i + 1])
            {
                // swap values
                temp = values[i];
                values[i] = values[i + 1];
                values[i + 1] = temp;
                swapped = 1;
            }
        }
    }
}


// selection sort: implement an O(n^2) sort
void
selection_sort(int values[], int n)
{  
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

/*
 * Returns true if value is in array of n values, else false.
 */

bool 
search(int value, int values[], int n)
{
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
search(int value, int values[], int n)
{

    // cut list in half
    int mid = round(n/2);
    
    if(values[mid] == value)
        // if number found, return true
        return true;
    else if (values[mid] > value)
        // key is in lower subset
        return search(value, values, n-1);
    else if (values[mid] < value)
        // key is in upper subset
        return search(value, values, n+1);

    return false;
}
*/

