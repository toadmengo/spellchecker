// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

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
const unsigned int N = 755;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // find the hash
    int index= hash(word);

    node *traverser = table[index]; //establish something that will traverse the links
    bool notsame = true; //this helps shutdown the loop if the word is found

    while (traverser!= NULL && notsame==true)
    {
        notsame= strcasecmp(traverser->word, word);
        traverser= traverser->next;
    }

    if (notsame ==true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // create hash function
    int a = tolower(word[0]);
    a=a-96;

    int nullcheck = word[1];
    int b;

    if (nullcheck!= 0)
    {
        b = tolower(word[1]);
        b=b-95;
    }
    else
    {
        b = 0;
    }
    int hashvalue= 28*a+b;

    return hashvalue;
}

int dsize = 0; //Records the amount of words in global variable dsize

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *f= fopen(dictionary, "r"); // file pointer to read the dictionary
    if (f== NULL)
    {
        return false;
        printf("f returned null");
    }

    // scan every string in dictionary untill EOF
    char word[LENGTH + 1];
    int index;

    while ((fscanf(f, "%s", word))!=EOF)
    {
        dsize++; //records size of dictionary

        node *temp= malloc(sizeof(node)); //create a node to copy strings into
        if (temp== NULL)
            {
                return false;
            }
        strcpy(temp->word, word);

        index= hash(word); //find the hash of the word

        //link hash table to new node
        if (table[index] == NULL)
        {
            temp->next= NULL;
            table[index] = temp;
        }
        else
        {
            temp->next = table[index];
            table[index] = temp;
        }

    }
    fclose(f);
    return true; //return true?
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    return dsize;
}

node *freer = NULL;
void linkfree(node *fr);

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // for loop over every index of table
    for (int i=0; i<N; i++)
    {
        freer = table[i];
        if (freer!= NULL)
        {
            linkfree(freer); // for the ith index, recursive free memory
        }
    }

    return true;
}

void linkfree(node *fr)
{

    if (fr->next!=NULL)
    {
        linkfree(fr->next);
    }
    free(fr);

}