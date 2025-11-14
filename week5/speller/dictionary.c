#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct word_node
{
    char word[LENGTH + 1];
    struct word_node *next;
} word_node;

// TODO: Choose number of buckets in hash table
const unsigned int HASH_TABLE_BUCKET_SIZE = 1000;

// Hash table
word_node *hash_table[HASH_TABLE_BUCKET_SIZE];

// counter for the total number of words added
int hash_table_word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // buffer for the normalize word
    char normalized_word[LENGTH + 1];
    // index so we can insert into the normalized string
    int char_index = 0;
    for (const char *cursor = word; *cursor != '\0' && char_index < LENGTH; cursor++)
    {
        normalized_word[char_index] = tolower((unsigned char) *cursor);
        char_index++;
    }
    // Add the final termination character for the string
    normalized_word[char_index] = '\0';

    // Hash the word and get the index for the hash table
    unsigned int bucket_index = hash(normalized_word);
    // get the current pointer to the latest word in the hash_table
    word_node *current_word = hash_table[bucket_index];
    // Now we can traverse the linked list looking for the word using strcasecmp
    while (current_word != NULL)
    {
        // keep comparing the words in the bucket until we don't have a cursor anymore
        // strcasecmp returns 0 if s1 is the same as s2, ignoring case
        if (strcasecmp(current_word->word, normalized_word) == 0)
        {
            // we found a match
            return true;
        }
        // update our current word to the next pointer and continue our loop
        current_word = current_word->next;
    }
    // it wasn't found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Implement djb2 hash using 5381 as a large prime number
    // function works by starting with a large prime number and then iterating over
    // each character in the string, multiplying the current hash value by 33, and adding the ASCII
    // value of the character.
    long hash_value = 5381;
    for (const char *char_ptr = word; *char_ptr != '\0'; char_ptr++)
    {
        // normalize the character
        unsigned char current_char = (unsigned char) *char_ptr;
        int lower_char = tolower(current_char);
        // create the updated hash value
        hash_value = (hash_value * 33 + lower_char);
    }
    // return the corresponding bucket based on the hash % bucket size
    unsigned int bucket_index = ((unsigned int) hash_value % HASH_TABLE_BUCKET_SIZE);
    return bucket_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary_path)
{
    // Open the dictionary file
    FILE *dictionary_file = fopen(dictionary_path, "r");

    if (dictionary_file == NULL)
    {
        // Something went wrong, log and exit
        return false;
    }
    // setup a buffer
    char word_buffer[LENGTH + 1];
    // Read each word in the file
    while (fscanf(dictionary_file, "%45s", word_buffer) != EOF)
    {
        // Add each word to the hash table
        word_node *new_word_node = malloc(sizeof(word_node));
        if (new_word_node == NULL)
        {
            // something went wrong allocating memory
            // Close the file
            fclose(dictionary_file);
            unload();
            return false;
        }
        // Create a word_node with the next pointer as null
        strcpy(new_word_node->word, word_buffer);
        new_word_node->next = NULL;

        // Hash the word and get the bucket
        unsigned int bucket_index = hash(word_buffer);
        // update the next pointer to the current pointer in the hash table
        // we do this because we are always adding to the front of the linked list, this ensures
        // our "next" pointer is pointing to the previous word.
        new_word_node->next = hash_table[bucket_index];
        // put the word into the proper bucket
        hash_table[bucket_index] = new_word_node;

        // Increment the word counter for size to return
        hash_table_word_count++;
    }
    // If we got here, things worked
    // Close the dictionary file and return true
    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // return the total word count added
    return hash_table_word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop all the buckets
    for (int i = 0; i < HASH_TABLE_BUCKET_SIZE; i++)
    {
        word_node *current_word = hash_table[i];
        // Traverse the linked list until we hit the last pointer
        while (current_word != NULL)
        {
            // free the memory
            word_node *word_to_free = current_word;
            // Assign current to the next node
            current_word = current_word->next;
            free(word_to_free);
        }
        // reset the current hash_table bucket back to null
        hash_table[i] = NULL;
    }
    // reset the hash_table_word_count
    hash_table_word_count = 0;
    // if we got here, things were freed up correctly, return true
    return true;
}
