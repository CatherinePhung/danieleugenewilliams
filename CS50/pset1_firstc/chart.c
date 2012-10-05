/****************************************************************************
 * chart.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program to promt a user for four non-­‐negative integers
 * (one for each of M spotting F, F spotting M, F spotting
 * F, and M spotting M), after which it should generate a
 * horizontal bar chart depicting those values, with the first 
 * value’s bar on top and the fourth value’s bar on the bottom.
 *
 * Created:  6/25/12
 * Modified: 6/26/12
 ***************************************************************************/

#include <cs50.h>
#include <stdio.h>

#define MAXBARLENGTH 80

int
main(void)
{
    // get M spotting F
    float mf = 0;
    do {
        printf("M spotting F: ");
        mf = GetFloat();
    } while (mf <= 0);
    
    // get F spotting M
    float fm = 0;
    do {
        printf("F spotting M: ");
        fm = GetFloat();
    } while (fm <= 0);
    
    // get F spotting F
    float ff = 0;
    do {
        printf("F spotting F: ");
        ff = GetFloat();
    } while (ff <= 0);
    
    // get M spotting M
    float mm = 0;
    do {
        printf("M spotting M: ");
        mm = GetFloat();
    } while (mm <= 0);
    
    // calc total number of sightings
    float total = mf + fm + ff + mm;
    
    // convert sighting to percentages
    
    // M spotting F percent
    float mf_per = mf/total;
    
    // F spotting M percent
    float fm_per = fm/total;
    
    // F spotting F percent
    float ff_per = ff/total;
    
    // M spotting M percent
    float mm_per = mm/total;
    
    printf("\nWho is Spotting Whom\n\n");
    
    // M spotting F Chart
    printf("M spotting F\n");
    for (int i = 0; i < (mf_per * MAXBARLENGTH); i++)
    {
        printf("#");
    }
    printf("\n");
    
    // F spotting M Chart
    printf("F spotting M\n");
    for (int i = 0; i < (fm_per * MAXBARLENGTH); i++)
    {
        printf("#");
    }
    printf("\n");
    
    // F spotting F Chart
    printf("F spotting F\n");
    for (int i = 0; i < (ff_per * MAXBARLENGTH); i++)
    {
        printf("#");
    }
    printf("\n");
    
    // M spotting M Chart
    printf("M spotting M\n");
    for (int i = 0; i < (mm_per * MAXBARLENGTH); i++)
    {
        printf("#");
    }
    printf("\n");
}
