// Implements a dictionary's functionality

#include <stdbool.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

unsigned int dictionary_size = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// const unsigned int N = 26;
#define N 26

// Hash table - +1 here is for the words that are single letter like a, b, or c
node *table[N][N+1];

void initialise_hashtable_pointers(void);
bool is_word_in_dict(const char *word, unsigned char index_1st, unsigned char index_2nd);
void assign_word_to_hashtable(node *n, char *word);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    bool word_exists;
    if (strlen(word) == 1)
    {
        unsigned char letter = tolower(word[0]) - 97;
        word_exists = is_word_in_dict(word, letter, 26);
        return word_exists;
    }
    else
    {
        unsigned char index_1st = tolower(word[0]) - 97;
        unsigned char index_2nd = tolower(word[1]) - 97;
        word_exists = is_word_in_dict(word, index_1st, index_2nd);
        return word_exists;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // since double index hash table is used, hash function is rendered useless
    return 0;
}

bool load(const char *dictionary)
{
    initialise_hashtable_pointers();

    char *line = NULL;
    size_t len = LENGTH;
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
        return false;
    }

    while (getline(&line, &len, fp) != -1) 
    {
        dictionary_size++;
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }
        // n->word = line; is done by assign_word_to_hashtable
        assign_word_to_hashtable(n, line);
        n->next = NULL;

        if (strlen(n->word) == 1)
        {
            unsigned char letter = line[0] - 97;
            table[letter][26] = n;
        }
        else
        {
            unsigned char letter_1st = line[0] - 97;
            unsigned char letter_2nd = line[1] - 97;
            if (table[letter_1st][letter_2nd] == NULL)
            {
                table[letter_1st][letter_2nd] = n;
            }
            else
            {
                node* prevptr = table[letter_1st][letter_2nd];
                while (true)
                {
                    if (prevptr->next == NULL)
                    {
                        prevptr->next = n;
                        break;
                    }
                    prevptr = prevptr->next; 
                }
            }
        }
    }
    // prints the words that start with re
    // for (node *n = table[17][4]; n != NULL; n = n->next)
    // {
    //     printf("%s\n", n->word);
    // }
    fclose(fp);
    if (line)
    {
        free(line);
        return true;
    }
    exit(EXIT_SUCCESS);
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i=0; i < N; i++)
    {
        for (int j=0; j < N+1; j++)
        {
            while (table[i][j] != NULL)
            {
                node *tmp = table[i][j]->next;
                free(table[i][j]);
                table[i][j] = tmp;
            }
        }
    }
    return true;
}

void initialise_hashtable_pointers(void)
{
    for (int i=0; i < N; i++)
    {
        for (int j=0; j < N + 1; j++)
        {
            table[i][j] = NULL;
        }
    }
}

bool is_word_in_dict(const char *word, unsigned char index_1st, unsigned char index_2nd)
{
    unsigned char result;
    bool is_in_dict = false;
    for (node *n = table[index_1st][index_2nd]; n != NULL; n = n->next)
    {
        result = strcasecmp(word, n->word);
        if (result == 0)
        {
            is_in_dict = true;
            break;
        }
    }

    return is_in_dict;
}

// Loads dictionary into memory, returning true if successful else false
void assign_word_to_hashtable(node *n, char *word)
{
    for (int i=0; word[i] != '\n'; i++)
    {
        n->word[i] = word[i];
    }
}
