/****************************************************************************
 * pennies2.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * The user will enter the the starting number of pennies 
 * and the number of days in the month and the program 
 * will calculate the amount of money the user will have 
 * received by the end of the month.
 * 
 * Created:  6/22/12
 * Modified: 6/25/12
 ***************************************************************************/

#include <cs50.h>
#include <stdio.h>
#include <string.h>

int
main(void)
{
    // get number of days in the month
    int days_in_month;
    while (true)
    {
        printf("Days in month: ");
        // get the user input
        days_in_month = GetInt();
        // if the user inputs 28, 29, 30, or 31, then continue.
        if (days_in_month == 28 || days_in_month == 29 || days_in_month == 30 || days_in_month == 31)
        {
            break;
        }
    }
    
    // get number of pennies on first day
    int pennies;
    while (true)
    {
        printf("Pennies on the first day: ");
        pennies = GetInt();
        // if pennies entered are positive, continue.
        if (pennies > 0)
        {
            break;
        }      
    }
    
    /*
     * get the total amount and print
     * out as U.S. currency in dollars and cents
     * while (days left in month):
     */
    long long total = pennies;
    int days_left = days_in_month;
    while (days_left >= 0)
    {
        // if (not first day of the month):
        if (days_left != days_in_month)
        {
            // double pennies
            total = 2 * total;
        }   
         
        // decrement days left in month
        days_left--;
    }

    // output in dollar and cents
    printf("You are the following amount at the end of the month:\n");
    printf("$%.2f\n", (float)total/100);
}
