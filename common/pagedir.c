/*
* Nikhil Pande
* COSC 50, 23S
* Professor Palmer
* Tiny Search Engine
*
* pagedir.c -- C program that handles initializing a directory
* and saving the HTML from crawled webages to it.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "webpage.h"
#include "file.h"
#include "mem.h"


/* function to initialize the pageDirectory. See 'pagedir.h' for more info */
bool pagedir_init(const char* pageDirectory)
{
    if (pageDirectory == NULL) {
        return false;
    }

    // create .crawler file
    FILE *fp;

    char* path = mem_malloc_assert((sizeof(char) * (strlen(pageDirectory) + strlen("/.crawler") ) + 1 ), "Path memory issue"); 

    sprintf(path, "%s/.crawler", pageDirectory); 
    // open file for writing
    if ((fp = fopen(path, "w") ) == NULL) { // if error free path and return false
        mem_free(path);
        return false; 

    } else {  // close file and return true
        mem_free(path);
        fclose(fp);
        return true;
  }

}


/* saves a webpage with an ID to the pageDirectory. See 'pagedir.h' for more info. */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    if (page != NULL && pageDirectory != NULL && docID > 0) {
        // floor (log10(docID)) calculates number of digits in the docID, then one more added (in +3) so we can allocate
        char* pathName = mem_malloc_assert((sizeof(char) * strlen(pageDirectory) + floor(log10(docID)) + 3), "Save pathName failed");

        sprintf(pathName,"%s/%d", pageDirectory, docID);
        FILE* fp;
        if ((fp = fopen(pathName, "w")) == NULL) {
            mem_free(pathName); // if open fails free pathName
        } else {
            fprintf(fp, "%s\n", webpage_getURL(page)); // print url
            fprintf(fp, "%d\n", webpage_getDepth(page)); // print depth
            fprintf(fp, "%s\n", webpage_getHTML(page)); // print HTML
            fclose(fp);
            mem_free(pathName);
        }
    }
}

/* takes a file in correct format and stores contents in a webpage_t. See 'pagedir.h' for more info. */
webpage_t* pagedir_readFile(FILE* fp)
{
    if (fp == NULL) {
        return NULL;
    }

    char* URL = file_readLine(fp); // first line
    char* depthChar = file_readLine(fp); // second line
    char* HTML = file_readFile(fp); // rest of file is HTML

    // turn depth into integer
    int depth;
    sscanf(depthChar, "%d", &depth);
    mem_free(depthChar); // free depthChar, rest is freed in webpage_delete in indexer

    webpage_t* page = webpage_new(URL, depth, HTML);
    return page;
}

/* validate if a pageDirectory is a valid crawler output. See 'pagedir.h' for more info. */
bool pagedir_validate(char* pageDirectory)
{
    if (pageDirectory == NULL) {
        return false;
    }

    FILE *fp;
    // sizeOfCharacter * (length of directory name + length of "/.crawler" (9) + null character (1))
    char* pathName = malloc( sizeof(char) * (strlen(pageDirectory) + 9) + 1); 
    sprintf(pathName, "%s/.crawler", pageDirectory); // create the name of the .crawler file

    if ((fp = fopen(pathName, "r")) != NULL) { // if this is a valid file, return true
        mem_free(pathName);
        fclose(fp);
        return true;
    } else {
        mem_free(pathName);
        return false;
    }
}

