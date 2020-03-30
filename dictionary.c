// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Count the dictionary size
int dsize = 0;



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // case sensitive
    int l = strlen(word);

    char tmp[l + 1];


    for (int i = 0; i < l; i++)
    {
        tmp[i] = tolower(word[i]);
    }

    tmp[l] = '\0';

    // take the word and hash the word
    int i = hash(tmp);

    // make a traverse linked list
    node *trav = table[i];

    while (trav != NULL)
    {
        if (strcasecmp(trav->word, tmp) == 0)
        {
            return true;
        }
        // move trav to the next node
        else
        {
            trav = trav->next;
        }
    }
    return false;
}



// Hashes word to a number
unsigned int hash(const char *word)
{
    //djb2, this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    // deterministic
    return hash % N;
}



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // OPEN dictionary file
    FILE *file = fopen(dictionary, "r");

    // make sure can open up the file
    if (file == NULL)
    {
        fclose(file);
        printf("cannot OPEN");
        return 1;
    }

    // eachword is a character array
    char eachword[LENGTH + 1];

    while (fscanf(file, "%s", eachword) != EOF)
    {
        // Create a new node to store each word
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            printf("Not enough memory");
            unload();
            return 1;
        }

        // copy string " " to the word
        strcpy(n->word, eachword);
        n->next = NULL;

        // Call upon hash function. Function takes a string and returns an index
        int i = hash(eachword);

        // Insert node into Hash Table
        if (table[i] != NULL)
        {
            n->next = table[i];
        }
        table[i] = n;

        dsize++;
    }
    fclose(file);
    return true;
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dsize;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // call free
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        node *next;

        while (current != NULL)
        {
          next = current;
          current = current->next;
          free(next);
        }
    }
    return true;
}
