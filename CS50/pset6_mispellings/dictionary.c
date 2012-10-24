/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 * Maximum words: 592 (dependent on memory)
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define HASHSIZE 1024

node *hashtable;

unsigned int dictionary_size = 0;

/*
 * Returns true if word is in dictionary else false.
 */

bool
check(const char *word)
{
    char *lword = (char *) malloc(sizeof(word));
    if(lword == NULL)
    {
        printf("Could not malloc lword: dictionary.c: 32\n");
        return false;
    }
    
    for(int i=0,n=strlen(word);i<n;i++)
    {
        lword[i] = tolower(word[i]);
    }
    
    int hashv = hash(lword);
    
    if(hashtable[hashv].next == 0x00)
    {
        free(lword);
        return false;
    }
    else
    {
        node *n; 
        n = hashtable[hashv].next;
        while(n != NULL)
        {
            if(strcmp(n->word, lword) == 0)
            {
                free(lword);
                return true;
            }
            n = n->next;
        }
    }
    
    free(lword);
    return false;
}


/*
 * Loads dictionary into memory.  Returns true if successful else false.
 */

bool
load(const char *dictionary)
{

    // initialize hashtable
    if(!init())
    {
        printf("Could not initialize hashtable.");
        return false; 
    }

    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    char word[LENGTH+1];
    
    while(fscanf(fp, "%s", word) == 1)
    {
        // count words in dictionary
        dictionary_size++;
        
        // get hash value for word
        int hashv = hash(word);
        
        // check that hash value is not greater than HASHSIZE
        if(hashv > HASHSIZE)
        {
            printf("hash value error: %d greate than %d\n", hashv, HASHSIZE);
            return false;
        }
        
        node *new_node;
        new_node = node_create(word);
        if(!new_node)
        {
            printf("could not create new node: Line 80");
            return false; 
        }
        
        node_push(new_node, hashv);
        //printf("HASH: %s: %d\n", word, hash(word));
    }
    
    fclose(fp);
    
    return true;
}


/*
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */

unsigned int
size(void)
{
    return dictionary_size;
}


/*
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

bool
unload(void)
{
    for(int i = 0; i < HASHSIZE; i++)
    {
        while(!stack_empty(i))
            node_pop(i);
    }
    
    free(hashtable);
    return true;
}

/*
 * Returns a hash value for a given word.
 */
int
hash(char *word)
{
    // hash value
    int hashv = 0;
    
    // for each character in word
    for(int i = 0, n = strlen(word); i < n; i++)
    {
        // if this is the first character
        if(i == 0)
            hashv += (word[i] << 1);
        else if((i != 0) && (i != n))
            hashv += (word[i] << 2);
    }
    
    // return hash
    return (hashv % HASHSIZE);
}

/*
 * Initialize hashtable with pointers NIL
 */
int
init(void)
{
    // malloc hashtable
    hashtable = (node *) malloc(HASHSIZE * sizeof(node));
    
    // test if memery was successfully allocated for hashtable
    if(hashtable != NULL)
        return 1;
    else
    {
        printf("Could not malloc hashtable: dictionary.c: 189\n");
        return 0;
    }
}

/*
 * create new node
 */
node*
node_create(char *word)
{
    // malloc node
    node *new_node;
    new_node = (node *) malloc(sizeof(node));
    if(new_node != NULL)
    {
        new_node->word = (char *)malloc(sizeof(char[strlen(word)+1]));
	    if(new_node->word != NULL)
	        strcpy(new_node->word, word);
    }

    return new_node;   
}


/* 
 * pushes the value onto the stack. 
 */
void
node_push(node *n, int hashv)
{
    // check if first element in hashtable[hashv] is null
    if(hashtable[hashv].next == 0x00)
    {
        //printf("hashtable[%d] is null\n", hashv);
        hashtable[hashv].next = n;
        
    } else
    {
        // there is already a node here.
        n->next = hashtable[hashv].next;
        hashtable[hashv].next = n;
    }
}


/*
 * Returns 1 if stack is empty. Returns 0 if stack is not empty.
 */
int 
stack_empty(int hashv)
{
    if (hashtable[hashv].next == NULL)
        return(1);
    else
        return(0);
}

/* 
 * Returns the top element of the stack, and removes that element. Returns garbage if the stack is empty. 
 */
int
node_pop(int hashv)
{
    node *temp;
    int popped = 0;
    if (hashtable[hashv].next != NULL)
    {
        temp = hashtable[hashv].next;
        hashtable[hashv].next = hashtable[hashv].next->next;
        free(temp->word);
        free(temp);
        popped = 1;
    } else
    {
       printf("test\n"); 
    }
    return(popped);
}
