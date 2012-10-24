/****************************************************************************
 * dictionary.h
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Declares a dictionary's functionality.
 ***************************************************************************/

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

typedef struct node
{
    char *word;
    struct node *next;
} node;

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45


/*
 * Returns true if word is in dictionary else false.
 */

bool check(const char *word);


/*
 * Loads dictionary into memory.  Returns true if successful else false.
 */

bool load(const char *dictionary);


/*
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */

unsigned int size(void);


/*
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

bool unload(void);


/*
 * Returns a hash value for a given word.
 */
int hash(char *name);


/*
 * Initialize hashtable with pointers NIL. Returns 0 if malloc fails.
 */
int init(void);

/*
 * create new node
 */
node* node_create(char *word);

/* 
 * pushes the value d onto the stack. 
 */
void node_push(node *n, int hashv);

/*
 * Returns 1 if stack is empty. Returns 0 if stack is not empty.
 */
int stack_empty(int hashv);

/* 
 * Returns the top element of the stack, and removes that element. Returns garbage if the stack is empty. 
 */
int node_pop(int hashv);

#endif // DICTIONARY_H
