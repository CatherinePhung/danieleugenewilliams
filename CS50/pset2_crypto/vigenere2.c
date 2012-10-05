/****************************************************************************
 * vigenere.c
 *
 * Computer Science 50
 * Daniel E. Williams
 *
 * A program that encrypts messages using Vigenere's cipher.
 * ciphertext = (pi + ki) % 26
 * plaintext
 * key
 *
 * Created:  6/28/12
 * Modified: 7/10/12
 ***************************************************************************/

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int Cipher(string, string, string);

int
main(int argc, string argv[])
{
    string key = "";
    // read keyword from command line
    // if (user enters no arguments || more than one arguments)
    if (argc < 2 || argc > 2)
    {
        // yell at user
        printf("You need to enter 1 keyword as an argument:  ./vigenere [string]\n");
        return 1;
    }
    else
    {
        // accept non-negative, command-line keyword, k
        key = argv[1];
    }
    
    // prompt for string to encode
    string plaintext = GetString();
    
    char ciphertext [strlen(plaintext)];
    
    // call Cipher function to print to screen
    Cipher(plaintext, key, ciphertext);

    printf("%s\n", (string)ciphertext);

}

int
Cipher(string plaintext, string key, string ciphertext)
{ 

    int p_val = 0;
    int k_val = 0;
    int c_val = 0;
    int k_cnt = 0;
    // loop over string
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if(isalpha(plaintext[i]))
        {
            
            k_val = 25 - (122 - tolower(key[k_cnt]));
            k_cnt++;
            // for each space, digit, or punct, I need to pause and not
            // use the current character in the key. This is what is causing
            // the problem.
            
            // is the letter uppercase
            if(plaintext[i] >= 65 && plaintext[i] <= 90)
            {
                // this is an adapatation of the modulus formula for ascii
                p_val = 26 - (91 - plaintext[i]);
                c_val = ((p_val + k_val)) % 26;
                ciphertext[i] = c_val + 'A';
            }
            // is the letter lowercase
            else if(plaintext[i] >= 97 && plaintext[i] <= 122)
            {
                // this is an adapatation of the modulus formula for ascii
                p_val = 26 - (123 - plaintext[i]);
                c_val = ((p_val + k_val)) % 26;
                ciphertext[i] = c_val + 'a';
            }
            
            // reset key
            if(key[k_cnt] == '\0')
                k_cnt = 0;
        }
        // if character is a space, print space
        else if(isspace(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }
        // if character is punctuation, print punct
        else if(ispunct(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }
        // if character is digit, print digit
        else if(isdigit(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }
        // else, just print the character, whatever it is...
        else
        {
            ciphertext[i] = plaintext[i]; 
        }
        
    }
    
    // end of string character
    ciphertext[strlen(plaintext)] = '\0';
    
    
    // output each encoded letter, making sure to not encode non-letters
    // This is CS50!
    // Mvue gl QE50!
    
    return 0;
}

