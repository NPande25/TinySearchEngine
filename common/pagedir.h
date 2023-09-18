/*
* Nikhil Pande
* COSC 50, 23S
* Professor Palmer
* Tiny Search Engine
*
* pagedir.h -- header file for pagedir.c
* C program that handles initializing a directory
* and saving the HTML from crawled webages to it.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "webpage.h"
#include "file.h"

/********* pagedir_init **********
* initializes page directory, creating .crawler file
*
* input:
*   user inputs a desired path name for the page directory
* returns:
*   opens file for writing, closes and returns true
*   on error returns false
*/
bool pagedir_init(const char* pageDirectory);

/************ pagedir_save ************
* saves a page to a given pagedirectory with its id number
* as the file name. Then prints to the file (in order):
*   URL
*   Depth
*   HTML
*
* inputs:
*   page (webpage_t*): valid webpage whose contents to save
*   pageDirectory (const char*): pageDirectory path name
*   docID (int): file id and name of the file
*
* outputs:
*   creates and writes new file in the pageDirectory, whose name 
*   is the docID
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);


/************ pagedir_readFile ************
* reads a file in the correct format (URL \n depth \n HTML)
* and stores its contents into a webpage data structure
*
* inputs:
*   fp (FILE*): file to get webpage information from
* output:
*   returns pointer to webpage_t with this data
*/
webpage_t* pagedir_readFile(FILE* fp);

/********* pagedir_validate **********
* validates that the directory passed in is actually a crawler
* output directory. Checks to see if there is a valid .crawler file
* 
* input:
*   name of crawler directory
* returns:
*   true if the directory is valid, false otherwsie
* 
*/
bool pagedir_validate(char* pageDirectory);