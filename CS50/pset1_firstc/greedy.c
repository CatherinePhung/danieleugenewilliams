/****************************************************************************
 * greedy.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program that asks the user hw much change is owed and then spits
 * out the minimum number of coins with which said change can be made.
 *
 * Created:  6/25/12
 * Modified: 6/25/12
 ***************************************************************************/

#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define QUARTER 25
#define DIME 10
#define NICKEL 5
#define PENNY 1

int
main(void)
{
    // counter to track coins
    int coin_counter = 0;
    
    // get money to make change for
    float f_owed = 0;
    do
    {
        printf("O, hai!  How much change is owed? ");
        f_owed = GetFloat();
    } while (f_owed <= 0);
    
    // convert money to cents
    int i_owed = 0;
    i_owed = round(f_owed * 100);
    
    // while (more than a quarter left):
    while (i_owed >= QUARTER)
    {    
        // subtract quarter
        i_owed -= QUARTER;

        // increment coins used
        coin_counter++;
    }
    
    // while (more than a dime left):
    while (i_owed >= DIME)
    {    
        // subtract dime
        i_owed -= DIME;

        // increment coins used
        coin_counter++;
    }   
    
    // while (more than a nickel left):
    while (i_owed >= NICKEL)
    {    
        // subtract nickel
        i_owed -= NICKEL;

        // increment coins used
        coin_counter++;
    }
    
    // while (more than a penny left):
    while (i_owed >= PENNY)
    {
        // subtract penny
        i_owed -= PENNY;

        // increment coins used
        coin_counter++;
    }

    // output coins used
    printf("%d\n", coin_counter);
}
