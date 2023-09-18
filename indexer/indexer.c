/*
* Nikhil Pande
* COSC 50, 23S, Professor Palmer
* Tiny Search Engine
*
* indexer.c - C program to implement the indexing subsystem
* of the tiny search engine 
*
*
*/

#include <stdlib.h>
#include <stdio.h>
#include "webpage.h"
#include "mem.h"
#include "hashtable.h"
#include "counters.h"
#include "index.h"
#include "pagedir.h"
#include "word.h"


static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);


/******** main **********
* parse command line, validate parameters, initialize modules
* build index and save it, exiting zero if successful
*
* Usage (from command line):
*   $ ./indexer <pageDirectory> <indexFileName>
* outputs:
*   the program creates a file in the directory that has an index
*   of all the words, in a format, with one word per line:
*       word docID1 count1 [docID2 count2] ... [docIDn countn]
*/
int main(int argc, char* argv[])
{
    char* pageDirectory = NULL;
    char* indexFileName = NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFileName);
    index_t* index = indexBuild(pageDirectory);
    index_save(index, indexFileName);
    index_delete(index);
    exit(0);
}


/*********** parseArgs **************
* parse the arguments of the command line and extract them into
* pageDirectory and indexFileName
*
* inputs:
*   argc (int): number of arguments
*   argv[] (char*): arguments as a character array
*   pageDirectory (char**): name of the directory where the webpage files are stored
*   indexFileName (char**): name of file to which the index output is written
* outputs:
*   argv[1] will be stored in pageDirectory, and argv[2] in indexFileName
* error handling:
*   if there are issues with parsing/normalization, we exit non-zero
*/
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFileName)
{

    // check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    // check for null arguments
    if (argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "At least one null argument\n");
        exit(2);
    }

    // check for validity of directory
    if (pagedir_validate(argv[1])) {
        *pageDirectory = argv[1];
    } else {
        fprintf(stderr, "Not a valid pageDirectory\n");
        exit(3);
    }

    // is the indexFileName valid?
    FILE* fp;
    if ((fp = fopen(argv[2], "w")) != NULL) {
        *indexFileName = argv[2];
        fclose(fp);
    } else {
        fprintf(stderr, "Not a valid indexFileName\n");
        exit(4);
    }


}

/******** indexBuild *********
* builds the index struct from the pageDirectory
* fetches each document file 'pageDirectory/id' and uses
* indexPage (below) to add its words to the index
* 
* input:
*   pageDirectory (char*): name of the directory with webpage files
* output:
*   pointer to a new index_t object with the data from the pageDirectory
*/
static index_t* indexBuild(char* pageDirectory)
{
    if (pageDirectory == NULL) { 
        return NULL;
    }

    index_t* index = index_new(600);
    mem_assert(index, "Error allocating index memory");

    int id = 1;
    FILE* fp;
    char* fileName = mem_malloc(sizeof(char) * (strlen(pageDirectory) + 6));
    sprintf(fileName, "%s/%d", pageDirectory, id);

    while ((fp = fopen(fileName, "r")) != NULL) {
        webpage_t* page = NULL;
    
        if ((page = pagedir_readFile(fp)) != NULL) {
            indexPage(index, page, id);
        }
        webpage_delete(page); // delete page we created
        id++;
        sprintf(fileName, "%s/%d", pageDirectory, id);
        fclose(fp);
    }
    mem_free(fileName);
    return index;

}

/******** indexPage *********
* takes a webpage and adds all of its words to the index, incrementing
* the count if they are already represented
* 
* input:
*   index (index_t*): pointer to the index
*   page (webpage_t*): the webpage we are indexing
*   docID (const int): the document ID of the webpage
* output:
*   index will be modified to include the account for the words in page
*/
static void indexPage(index_t* index, webpage_t* page, const int docID)
{
    if (index != NULL && page != NULL) { // defense
        int pos = 0;
        char* word;
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {

            if (strlen(word) >= 3) { // skip trivial words (length < 3)

                normalizeString(word); // normalize
                if (index_insert(index, word, docID)) { // lookup and add if necessary
                }
            }
            mem_free(word);
        }
    }
}
