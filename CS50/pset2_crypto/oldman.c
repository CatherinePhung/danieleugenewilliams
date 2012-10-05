/****************************************************************************
 * oldman.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program to print "This old man..." in the most efficient
 * manner.
 *
 * Created:  6/27/12
 * Modified: 6/28/12
 ***************************************************************************/
#include <cs50.h>
#include <stdio.h>

void ThisOldman(int); /* Prototype to print ten this old man verses */
void Knickknack();    /* Prototype to print same knick-knack verse ten times */

int
main(void)
{
    // loop through ten times to call ThisOldman and Knickknack functions
    for (int i = 0; i < 10; i++)
    {
        ThisOldman(i);
        Knickknack();
        
        // print an additional line after each verse
        printf("\n");
    }
}

/* 
 * ThisOldman function stores ten lines of custom verses 
 * in a string array. The function takes an int argument
 * and prints the corresponding array item to the screen.
 */
void 
ThisOldman(int verse)
{
    // oldman array
    string oldman[10];
    oldman[0] = "This old man, he played one\nHe played knick-knack on my thumb";
    oldman[1] = "This old man, he played two\nHe played knick-knack on my shoe";
    oldman[2] = "This old man, he played three\nHe played knick-knack on my knee";
    oldman[3] = "This old man, he played four\nHe played knick-knack on my door";
    oldman[4] = "This old man, he played five\nHe played knick-knack on my hive";
    oldman[5] = "This old man, he played six\nHe played knick-knack on my sticks";
    oldman[6] = "This old man, he played seven\nHe played knick-knack up in heaven";
    oldman[7] = "This old man, he played eight\nHe played knick-knack on my gate";
    oldman[8] = "This old man, he played nine\nHe played knick-knack on my spine";
    oldman[9] = "This old man, he played ten\nHe played knick-knack once again";
    
    // only print if verse argument is between 0 and 9; else print blank line
    if (verse > 0 || verse < 10)
        printf("%s\n", oldman[verse]);
    else
        printf("\n");
}

/*
 * Knickknack function print knickknack string to the screen for each verse.
 */
void
Knickknack()
{
    printf("Knick-knack paddywhack, give your dog a bone\nThis old man came rolling home\n");
}




