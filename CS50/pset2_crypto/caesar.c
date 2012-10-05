/****************************************************************************
 * caesar.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program that encrypts messages using Caesar's cipher.
 * c = (pi + k) % 26
 * c = ciphertext
 * p = plaintext
 * k  = secret key
 *
 * Created:  6/28/12
 * Modified: 7/3/12
 ***************************************************************************/

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string Cipher(string, int); // function to rotation string by k characters

int
main(int argc, string argv[])
{
    int k = 0;
    
    // if (user enters no arguments || more than one arguments)
    if (argc == 0 || argc == 1 || argc > 2)
    {
        // yell at user
        printf("You need to enter 1 integer as an argument:  ./caesar [int]\n");
        return 1;
    }
    else
    {
        // accept non-negative, command-line argument, k
        k = atoi(argv[1]);
        k = k * 1;
    }
    
    // prompt user for plaintext  
    printf("Enter the text that you would like to encrypt:\n");
    string p = GetString();
    
    string s = Cipher(p, k);
    printf("%s\n", s);
    
    return 0;    
}


// rotate plaintext p by k positions; 
string
Cipher(string p, int k)
{  
    int c = 0;
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        
        // is the letter uppercase
        if(p[i] >= 65 && p[i] <= 90)
        {
            c = ((26 - (91 - p[i]) + k) % 26);
            p[i] = c + 'A';
        } 
        // is the letter lowercase
        else if(p[i] >= 97 && p[i] <= 122)
        {
            c = ((26 - (123 - p[i]) + k) % 26);
            p[i] = c + 'a';
        } 
        else if(isspace(p[i]))
        {
            // BLANK SPACE; do nothing
        }
        else if(ispunct(p[i]))
        {
            // punct; do nothing
        }
        else if(isdigit(p[i]))
        {
            // DIGIT; do nothing
        }
        else
        {
            // ELSE PRINT BLANK 
        }
    }
    return p;
}

