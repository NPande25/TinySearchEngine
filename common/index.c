/*
* Nikhil Pande
* COSC 50, 23S
* Tiny Search Engine
*
* index.c - index module for the indexer subsystem
* see index.h for more details.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hashtable.h"
#include "mem.h"
#include "file.h"
#include "counters.h"
#include "index.h"


/******** global structs *********/
typedef struct index {
    hashtable_t* table;
} index_t;
/*************/

typedef struct counters counters_t;


index_t* index_new(const int numSlots);
bool index_insert(index_t* index, const char* word, const int ctr_key);
bool index_save(index_t* index, char* fileName);
bool index_delete(index_t* index);


/*********** local functions to help with saving ***********/
static void save_counter(void* arg, const int key, const int count) 
{
    FILE* fp = arg;
    fprintf(fp, "%d %d ", key, count);
}

static void save_table(void* arg, const char* key, void* item) 
{
    FILE* fp = arg;
    fprintf(fp, "%s ", key);
    counters_iterate(item, fp, save_counter);
    fprintf(fp, "\n");
}

// need a specific delete table function that takes a parameter of type void* rather than just counters_t*
static void delete_table(void* item) {
    counters_delete(item);
}

static void index_print_helper(FILE* fp, const char* key, void* item)
{
    fprintf(fp, "%s", key);
}




/************ global functions***********/
/* create new index */
index_t* index_new(const int numSlots)
{
    if (numSlots < 0) { // defense
        fprintf(stderr, "Invalid numSlots in index_new");
        return NULL;
    }
    
    index_t* index = mem_malloc(sizeof(index_t));
    mem_assert(index, "Index allocation error");
    index->table = hashtable_new(numSlots);
    return index;
}

/* insert a word into hashtable or increment its count */
bool index_insert(index_t* index, const char* word, const int ctr_key)
{
    if (index == NULL || word == NULL || ctr_key == 0) { // defense
        return false;
    }

    counters_t* wordCounter;

    if ((wordCounter = hashtable_find(index->table, word)) != NULL) { // if word is in the index
        counters_add(wordCounter, ctr_key); // note one more occurrence of word in document with ID = ctr_key
        return true;
    } 
    else { // word isn't there yet
        wordCounter = counters_new();
        counters_add(wordCounter, ctr_key);
        hashtable_insert(index->table, word, wordCounter);
        return true;
    }

    return false;  // if other processes doesn't succeed
}

/* gets the counter associated with a certain word */
counters_t* index_find(index_t *index, char *word)
{
  return hashtable_find(index->table, word); 
}

/* save the index to an output file */
bool index_save(index_t* index, char* fileName) 
{
    if (index == NULL || fileName == NULL) {
        return false;
    }
    
    FILE* fp;
    if ((fp = fopen(fileName, "w")) != NULL) { // if we successfully open file
        hashtable_iterate(index->table, fp, save_table);
    }
    else {
        fprintf(stderr, "File cannot be opened\n");
        return false;
    }
    fclose(fp);
    return true;
}

void index_print(index_t* index, FILE* fp)
{
    hashtable_print(index->table, fp, index_print_helper);
}

/* delete the index structure */
bool index_delete(index_t* index) {

    if (index != NULL) {
        hashtable_delete(index->table, delete_table);
        mem_free(index);
        return true;
    }
    return false; // if the above fails

}

/* load index from file */
index_t* index_loadIndex(char* oldIndexFilename) {

    if (oldIndexFilename == NULL) {
        return NULL;
    }
    
    // read in index file 
    FILE *fp;
    if ((fp = fopen(oldIndexFilename, "r")) != NULL) {
        int numWords = file_numLines(fp);
        index_t* index = index_new(numWords);

        int docID, count;
        char* word;
        counters_t* ctr; 

    // loops through the lines 
        for(int i = 0; i < numWords ; i++) {
            ctr = counters_new();
            word = file_readWord(fp);

            // go through each pair of numbers
            while (fscanf(fp, "%d %d ", &docID, &count) == 2 ){
                counters_set(ctr, docID, count); 
                hashtable_insert(index->table, word, ctr);
            }
            mem_free(word);
        }

        fclose(fp);
        return index;

    } else {
        fprintf(stderr, "Invalid oldIndexFilename\n");
        return NULL;
        exit(4);
    }
}





