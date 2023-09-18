/*
* Nikhil Pande
* COSC 50, 23S
* Tiny Search Engine
*
* index.h - header file for index.c exported functions
*
*/

#include "counters.h"


/****** GLOBAL TYPES *****/
typedef struct index index_t;

/**************** functions ****************/
/********** index_new ***********/
/* create a new index structure
*
* caller provides:
*   number of slots in their desired index
* we return:
*   an initialized index structure
* We guarantee:
*   counterset is intialized empty.
* caller is responsible for:
*   later calling index_delete to free the memory
*/
index_t* index_new(const int numSlots);


/*********** index_insert *************/
/* insert a word, counter key pair into the index
*
* Caller provides:
*   valid pointer to index, and word and counter key
* We return:
*   true if the insertion is successful, false if not
* We do:
*   if the word does not yet exist in the index, insert it and put its docID, count pair in.
*   if the word does exist, check if counter structure has the docID yet.
*       if the docID is there, increment count. If it isn't, add new docID and initialize count to 1
*       (all done in counters_add)
*/
bool index_insert(index_t* index, const char* word, const int ctr_key);


/******* index_find **********
* returns the counter associated with a certain word
* 
* caller provides:
*   index to look through
*   word to find
* we return:
*   the counter for the word, which holds (docID, count) pairs
*/
counters_t* index_find(index_t* index, char* word);

/*********** index_save ***********/
/* Uses the save_table helper function to iterate over the hashtable
* and print the index to a file in the desired format
* Utilizes the following pseudocode:
*    check args
*    create file fp
*    if you can, open file with fileName
*        hashtable iterate a save_table function
*    else say it failed
*    close file
* Caller provides:
*   index to save and name of the resulting file
* We guarantee:
*   a file will be created that displays the index in the following form:
*   one line per word, one word per line.
*   line structure: word docID1 count1 [docID2 count2] ... [docIDn countn]
* We return:
*   true if process is successful, false otherwise
*/
bool index_save(index_t* index, char* fileName);

/*********** index_print ***********
* print function used for debugging. Prints the hashtable
* and all words in it in their corresponding slots
*
* inputs:
*   index (index_t*): the index to print
*   fp (FILE*): file to which it will be printed
* output:
*   the hashtable will be printed, with each slot showing
*   the words that hashed to that slot
*/
void index_print(index_t* index, FILE* fp);


/*********** index_delete ***********
* delete the index created in index_new
* Caller provides:
*   a valid pointer to counterset.
* We do:
*   we ignore NULL indexes.
*   we free all memory we allocate for this index.
*/
bool index_delete(index_t* index);


/********** index_loadIndex *********
 * load an index from an index file. The index must
 * be in a very specific format:
 *      word [docID1 count1] ... [docIDn countn]
 * 
 * input:
 *    file name for the index we are reading
 * output:
 *    the index that we created from reading the file
 */
index_t* index_loadIndex(char* oldIndexFilename);
