/*
* Nikhil Pande
* COSC 50, 23S
* Professor Palmer
*
* crawler.c -- C program that executes the crawler portion of
* the tiny search engine. The program takes a root webpage,
* reads it, and recursively repeats this process for all
* adjacent linked webpages, up to a certain depth. 
* Note: this filters for only Dartmouth internal webpages,
*  keeping unwanted traffic from unsuspecting webpages.
*
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"
#include "mem.h"

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);


/*
* main function outsources much of the activity to helper functions,
* such as parseArgs and crawl. In our case, the main funciton simply
* checks that it has the correct amount of arguments, initializes the
* variables, calls parseArgs and crawl, and exits.
*
* inputs (from command line):
*   $ ./crawler <ROOT_URL> <DIRECTORY> <MAX_DEPTH>
* 
* outputs:
*   this function crawls from the specified root to a depth
*   of maxDepth, saving the HTML for each page to a directory
*/
int main(const int argc, char* argv[])
{
    // check arguments
    if (argc != 4) { // must have 3 args
        fprintf(stderr, "Invalid number of arguments.\n");
        exit(1);
    }

    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
    exit(0);
}

/*
************** parseArgs **************
* The parseArgs function takes the program's arguments as input,
* checks their validity, and saves them into the function's other
* parameters. Specifically, the program ensures that the seed URL
* is internal, that the inputted maxDepth is within a specified range,
* and that the page directory is valid. It also normalizes the URL before
* saving it into seedURL.
*
* inputs:
*   argc: number of arguments passed into program
*   argv[]: arguments in a character array
*   seedURL (char**): the root URL from which to begin crawl
*   pageDirectory (char**): directory to save files to
*   maxDepth (int*): maximum depth of the crawl.
*
* outputs:
*   function validates and normalizes the arguments
*
* error handling:
*   in the case of an error with any of the arguments, the function prints
*   to stderr and exits non-zero.
*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth)
{
    if (!isInternalURL(argv[1])) {
        fprintf(stderr, "Seed URL is NULL or not internal\n");
        // mem_free(*seedURL);
        exit(3);
    }
    if (sscanf(argv[3], "%i", maxDepth) == 0) { // if works, sets third arg to maxDepth
        fprintf(stderr, "maxDepth not valid integer\n");
        mem_free(*seedURL);
        exit(4);
    }
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "maxDepth value out of range");
        mem_free(*seedURL);
        exit(5);
    }
    if (pagedir_init(argv[2]) == false) {
        fprintf(stderr, "invalid pageDirectory.\n");
        mem_free(*seedURL);
        exit(6);
    }

    // if all succeeds
    *seedURL = normalizeURL(argv[1]); // normalize
    *pageDirectory = argv[2]; // set directory to second arg
}
        


/*
******************** crawl *********************
* The crawler function crawls from the root url seedURL to 
* a given depth maxDepth. The program  begins by scanning the 
* seedURL, reading its HTML and saving it to a new file 
* in pageDirectory. The program inserts this webpage into
* a new hashtable to track all the pages it has visited. Then, 
* the file collects all the links to other pages in the HTML 
* and inserts them into a bag. While this bag is not empty (while
* there are still more pages in the bag), the program extracts a page 
* from the bag, reads it, and, again, saves it in the hashtable and adds 
* its unvisited linked pages to the bag. 
*
* inputs: 
*   seedURL (string): root URL from where the crawl begins
*   pageDirectory (string): path name for the directory to which we are saving the webpages
*   maxDepth (const int): depth of our crawl, how many layers of links we wish to go to
*
* outputs:
    exports pages found during its crawl to pageDirectory
*
* error handling:
*   if memory allocation fails at any point, the function will terminate.
*   This does not free previously allocated memory, so this could result
*   in memory leaks.
*
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{

    // initialize the hashtable and add the seedURL
    hashtable_t* visited = hashtable_new(200);
    hashtable_insert(visited, seedURL, "");

    // initialize the bag and add a webpage representing the seedURL at depth 0
    int id = 0;
    bag_t* pages = bag_new();
    webpage_t* root = webpage_new(seedURL, 0, NULL); // root webpage
    bag_insert(pages, root);
    id++;

    webpage_t* next = NULL;

    // while bag is not empty
    // 	pull a webpage from the bag
    while ((next = bag_extract(pages)) != NULL) {
        
        //	fetch the HTML for that webpage
        //	if fetch was successful,
        if (webpage_fetch(next)) {
            logr("Fetched", webpage_getDepth(next), webpage_getURL(next));

            // save the webpage to pageDirectory
            pagedir_save(next, pageDirectory, id);

            // if the webpage is not at maxDepth,
            if (webpage_getDepth(next) < maxDepth) {
             //	pageScan that HTML
                logr("Scanning", webpage_getDepth(next), webpage_getURL(next));
                pageScan(next, pages, visited);
            }
            id++;
        } 
        //	delete that webpage
        webpage_delete(next);
    }
    
    // delete the hashtable and bag
    hashtable_delete(visited, NULL);
    bag_delete(pages, webpage_delete);
}


/*
* pageScan function reads the HTML from a webpage and identifies
* links to other webpages. The function uses the webpage_getNextURL
* method from the webpage module, and if the URL is internal, it is
* inserted into the hashtable. Then, a webpage for it is created, whose
* depth is one greater than the page it is on, and this new webpage is
* added to the bag of pages to crawl.
* 
* inputs:
*   page (webpage_t*): the page we are currently scanning
*   pagesToCrawl (bag_t*): the bag of pages we need to crawl
*   pagesSeen (hashtable_t*): hashtable of URLs that we've visited
*
* outputs:
*   adds new, internal URLs to hashtable and inserts the corresponding
*   webpages to the bag
*
* error handling:
    if any of the input parameters are NULL, the function does nothing
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    if (page != NULL && pagesToCrawl != NULL && pagesSeen != NULL) { // check parameters

        int pos = 0;
        char* next_url; 
        char* norm_next;
        int depth = webpage_getDepth(page) + 1; // depth of new found URLs


        // while there is another URL in the page
        while ((next_url = webpage_getNextURL(page, &pos)) != NULL) {
            
            if ((norm_next = normalizeURL(next_url)) != NULL) {

                logr("Found", depth-1, norm_next);

                if (isInternalURL(norm_next)) { // if url is internal

                    if (hashtable_insert(pagesSeen, norm_next, "")) { // if insert works

                        logr("Added", depth-1, norm_next);
                        // create a webpage_t for it and insert into bag
                        webpage_t* next_page = webpage_new(norm_next, depth, NULL);
                        bag_insert(pagesToCrawl, next_page);

                    } else { // URL already in hashtable
                        logr("IgnDupl", depth-1, norm_next);
                        // ** if bag insert doesn't work, must free norm_next ourselves instead of webpage delete
                        mem_free(norm_next);
                    }
                } else { // URL not internal
                    logr("IgnExtrn", depth-1, norm_next);
                    mem_free(norm_next); // if external, need to free now. see ** in line 236
                }
            }

            mem_free(next_url);

        }
    }
}

// log one word (1-9 chars) about a given url
// function borrowed from class notes: https://www.cs.dartmouth.edu/~tjp/cs50/labs/lab4/IMPLEMENTATION                                  
static void logr(const char *word, const int depth, const char *url)
{
// #ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
// #else
//   ;
// #endif
}
