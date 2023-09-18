/*
* Nikhil Pande
* COSC 50, 23S, Professor Palmer
* Tiny Search Engine
*
* indextest.c - C program to test the indexer subsystem
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

static void parseArgs(int argc, char* argv[], char** oldIndexFilename, char** newIndexFilename);


int main (int argc, char* argv[])
{
    // initialize
    char* oldIndexFilename;
    char* newIndexFilename;

    // parse args
    parseArgs(argc, argv, &oldIndexFilename, &newIndexFilename);

    index_t* index = index_loadIndex(oldIndexFilename);
    // save and delete index
    index_save(index, newIndexFilename);
    index_delete(index);



}

static void parseArgs(int argc, char* argv[], char** oldIndexFilename, char** newIndexFilename)
{
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments");
        exit(1);
    }
    
    if(argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "one or more arguments is Null");
        exit(2);
    }

    FILE* fp;
    if ((fp = fopen(argv[1], "r") ) != NULL) {
        *oldIndexFilename = argv[1];
        fclose(fp);
    } else {
        free(*oldIndexFilename);
        fprintf(stderr, "invalid oldIndexFilename\n");
        exit(2);
    }

    if ((fp = fopen(argv[2], "w")) != NULL) {
        *newIndexFilename = argv[2];
        fclose(fp);
    } else {
        free(*newIndexFilename);
        fprintf(stderr, "invalid newIndexFilename\n");
        exit(3);
    }

}