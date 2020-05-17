// Implements a dictionary's functionality

#include <strings.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 33; // Recommended key number from hash function source

// Hash table initialized to null
node *table[N] = {NULL};

// Declare a global counter for words loaded
unsigned int wordcount = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word to obtain hash index
    unsigned int hashindex = hash(word);
    // Create traversal pointer to search through list
    node *trav = table[hashindex];
    // Search until you hit a null pointer
    while (trav != NULL)
    {
        if (strcasecmp(trav->word, word) == 0)
        {
            return true;
        }
        // If the word doesn't match, set the trav pointer to the next node
        trav = trav->next;
    }
    return false;
}

/*
* Hash function "djb2"
* Citation: http://www.cse.yorku.ca/~oz/hash.html
* Reported by Dan Bernstein
* Modified to be case insensitive
*/
unsigned int hash(const char *word)
{
    // Convert the word to lowercase first (copy into arr first)
    int len = strlen(word);
    char wordarr[len + 1];
    strcpy(wordarr, word);
    for (int i = 0; wordarr[i]; i++)
    {
        wordarr[i] = tolower(wordarr[i]);
    }

    unsigned long hash = 5381;
    int c = 0;

    for (int i = 0; c == wordarr[i]; i++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N; // Ensures within range
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }

    // Initialize a char array to store each word scanned
    char getword[LENGTH + 1];

    // Read the strings until end of file
    while (fscanf(fp, "%s", getword) != EOF)
    {
        // Allocate memory for the node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        // Assign values into the node using hash
        unsigned int hashindex = hash(getword);
        strcpy(n->word, getword);
        n->next = table[hashindex]; // returns NULL if this is the first node
        table[hashindex] = n;

        // Increase wordcount
        wordcount++;
    }
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Iterate through each hash index
    for (int i = 0; i < N; ++i)
    {
        // Create two pointers: one for freeing, one for keeping track of next node
        node *freeptr = table[i];
        node *cursor = table[i];
        // Loop through the list, freeing each node
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(freeptr);
            freeptr = cursor;
        }
    }
    return true;
}
