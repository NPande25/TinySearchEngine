/* Nikhil Pande
* COSC 50, 23S
* Tiny Search Engine
*
* querier.c - file to implement the querier subsystem
* of the Tiny Search Engine
*
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "mem.h"
#include "index.h"
#include "query.h"
#include "counters.h"
#include "pagedir.h"
#include "readlinep.h"
#include "file.h"
#include "webpage.h"

/**************** local types ****************/
typedef struct query {
    char* string;
    char** string_tok;
    int wordcount;
} query_t;

typedef struct document {
    int id;
    int score;
} document_t;



static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFileName);
static char* normalize_query(query_t* query);
static void tokenize_query(query_t* query);
static void querier(char* pageDirectory, char* indexFilename);
static char* getNextWord(const char* str, int* pos);
static bool validate_query(query_t* query);
static counters_t* scoreQuery(query_t* query, index_t* index);
static bool ctr_is_empty(counters_t* ctr);
static void isempty_helper(void* arg, const int key, const int count);
static void makeDocArray(counters_t* ctr, document_t** docArray, const int docCount);
static void putDoc(void* arg, const int key, const int count);
static void query_delete(query_t* query);

int fileno(FILE *stream);
static document_t* document_new(void);




int main(int argc, char* argv[])
{
// initalize pd, indexfilnaem
// parse arguments
// run querier
    char* pageDirectory = NULL;
    char* indexFileName = NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFileName);

    querier(pageDirectory, indexFileName);
    
    exit(0);

}

/*********** parseArgs **************
* parse and validate the arguments of the command line and extract them into
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
    if ((fp = fopen(argv[2], "r")) != NULL) {
        *indexFileName = argv[2];
        fclose(fp);
    } else {
        fprintf(stderr, "Not a valid indexFileName\n");
        exit(4);
    }


}

/********* querier ***********/
static void querier(char* pageDirectory, char* indexFileName)
{
/*
    defense on args
    load index from indexFileName
    prompt

    while (readline) not EOF
        initialize query structure
        normalize passed-in query
        tokenize query
        parse and score query, return a counter
        count how many results there are
        create/initialize an array of document types, size = count above
        add documents, put in the html
        delete result counter
        print the documents
*/

    if (pageDirectory == NULL || indexFileName == NULL) {
        fprintf(stderr, "Null arguments for querier");
        exit(5);
    }

    index_t* index = index_loadIndex(indexFileName);

    printf("Query: ");

    char* in;
    while ((in = readLinep()) != NULL) { // read until EOF
        
        // intialize query structure
        query_t* query = mem_assert(malloc(sizeof(query_t)), "query creation");
        query->string = in;
        query->string_tok = NULL;
        query->wordcount = 0;

        // normalize it
        char* normal;
        if ((normal = normalize_query(query)) == NULL) {
            printf("Query: %s\n", in);
            printf("--------------------------------------------\n");
            printf("Query: ");
            mem_free(query);
            mem_free(in);
            continue;
        }
        query->string = normal;
        mem_free(in);
        printf("Query: %s\n", normal);

        // tokenize it
        tokenize_query(query);

        // validate it
        if (!validate_query(query)) {
            printf("--------------------------------------------\n");
            printf("Query: ");
            query_delete(query);
            continue;
        }

        // score it
        counters_t* scores = scoreQuery(query, index);

        // free all the query data now that we have counters
        query_delete(query);
        if (scores == NULL) {
            free(scores);
            printf("No matches found\n");
            printf("--------------------------------------------\n");
            printf("Query: ");
            continue;
        }

        // count result
        int docCount = query_count(scores);

        document_t** docArray = calloc(docCount, sizeof(document_t*));
        for (int i = 0; i < docCount; i++) {
            docArray[i] = document_new();
            docArray[i]->id = 0;
        }
        makeDocArray(scores, docArray, docCount);
        printf("Matches %d document(s) (ranked):\n", docCount);
        for (int j = 0; j < docCount; j++) {
            FILE* fp;
            char* pathName = mem_malloc_assert((sizeof(char) * strlen(pageDirectory) + floor(log10(docArray[j]->id)) + 3), "Save pathName failed");
            sprintf(pathName, "%s/%d", pageDirectory, docArray[j]->id);
            if ((fp = fopen(pathName, "r")) != NULL) {
                webpage_t* page = pagedir_readFile(fp);
                printf("Score: %4d Doc: %4d %s\n", docArray[j]->score, docArray[j]->id, webpage_getURL(page));
                mem_free(pathName);
                webpage_delete(page);
                fclose(fp);
            } else {
                fprintf(stderr, "%s pathname failed", pathName);
            }
        }
        printf("--------------------------------------------\n");
        printf("Query: ");


        // FREE
        counters_delete(scores);
        for (int i = 0; i < docCount; i++) {
            mem_free(docArray[i]);
        }
        mem_free(docArray);


    }

    index_delete(index);
    printf("\n");

}


/********* normalize_query **********/
/*
* normalize the inputted query
* 
*
*/
static char* normalize_query(query_t* query)
{
    // defense
    // initialize new string, integers, length of string
    //      bool variable followSpace to track if we're in the middle of the word, avoiding extra spaces
    // allocate memory for new string

    // for every character
    //    if not letter or space
    //      print error, free, exit
    //    else
    //      if letter
    //          make lowercase and add at index to new array
    //      if space
    //          if not followSpace
    //              add at index to new word
    //              set followSpace = true
    //
    // end for
    // return the new query

    if (query == NULL) {
        fprintf(stderr, "Query is null");
        return NULL;
    }

    int normIdx = 0; // index in the normalized character array. Length could change because we remove duplicate spaces
    char* qString = query->string;
    int length = strlen(qString);
    bool followSpace = false; // variable to track if the previous character was a space, so we remove duplicate spaces
    char* normQuery = calloc((length + 1), sizeof(char));
   
   
    for (int i = 0; i < length; i++) {
        if (!isspace(qString[i]) && !isalpha(qString[i])) { // if not letter or space
            fprintf(stderr, "Invalid character in query\n");
            mem_free(normQuery);
            return NULL;
        } else {
            if (isalpha(qString[i])) {
                normQuery[normIdx] = tolower(qString[i]);
                followSpace = false;
                normIdx++;
            }
            else if (isspace(qString[i])) {
                if (!followSpace) {
                    followSpace = true;
                    normQuery[normIdx] = qString[i];
                    normIdx++;
                    query->wordcount++;
                }
            }
        }
    }

    query->wordcount++;
    return normQuery;
    
}

/******* getNextWord *******/
/*
* get the next word in the query
* adapted from the webpage_getNextWord function
* in ../libcs50/webpage.c 
*/
static char* getNextWord(const char* str, int* pos)
{
    if (str == NULL || pos == NULL) {
    	return NULL;
    }

    const char* beg; // beginning character
    const char* end; // ending character
    const char null = '\0';


    // skip non-alphabatic characters
	while (str[*pos] != null && !isalpha(str[*pos])) {
		(*pos)++;
	}
	// if this is the end of the string
	if (str[*pos] == null) {
		return NULL;
	}

	// str[*pos] is the first character of a word
	beg = &(str[*pos]);
	
    // go to the end of the word
    while (str[*pos] != null && isalpha(str[*pos])) {
        (*pos)++;
	}
	// at this point, str[*pos] is the first character *after* the word.
	// so str[*pos-1] is the last character of the word.
	 end = &(str[*pos-1]);
	 // 'beg' points to first character of the word
	 // 'end' points to last character of the word
    int wordlen = end - beg + 1;

    // allocate space for length of new word + '\0'
    char* word = mem_assert(calloc(wordlen + 1, sizeof(char)), "readword");
    if (word == NULL) {        // out of memory!
    	return NULL;
    } else {
        // copy the new word
        strncpy(word, beg, wordlen);
        return word;
    }
}

/********** tokenize_query *********/
/* 
* function to tokenize the query, using getNextWord
* to create a list of the words in the query
*/
static void tokenize_query(query_t* query)
{
    char* qString = query->string;
    query->string_tok = calloc((query->wordcount + 1), sizeof(char*));
    int pos = 0;
    int wordNum = 0; // which word we're at, updates in while loop

    char* word;
    while ((word = getNextWord(qString, &pos)) != NULL) {
        query->string_tok[wordNum] = malloc(strlen(word) + 1);
        strcpy(query->string_tok[wordNum], word);
        wordNum++;
        mem_free(word);
    }
}

static bool validate_query(query_t* query)
{
/*
    tokenize query
    check if first or last word is and/or
    for each word
        if word is first or last
            check if its and/or
        check if next one is and/or
*/

    for (int i = 0; i < query->wordcount; i++) {
        if (strcmp(query->string_tok[i], "and") == 0 || strcmp(query->string_tok[i], "or") == 0) {
            // check if query starts or ends with and/or.
            if (i == 0 || i == query->wordcount - 1) {
                fprintf(stderr, "Error: there cannot be trailing or leading logical " \
                                "operators in the query.\n");
                return false;
            // check if previous/next word is and/or. Cannot have consecutive logical operators.
            } else if (strcmp(query->string_tok[i-1], "and") == 0 || 
                        strcmp(query->string_tok[i-1], "or")  == 0 ||
                        strcmp(query->string_tok[i+1], "and") == 0 ||
                        strcmp(query->string_tok[i+1], "or")  == 0) {
                fprintf(stderr, "Error: there cannot be consecutive logical " \
                                "operators in the query.\n");
                return false;
            }
        }
    }
    return true;
}


static counters_t* scoreQuery(query_t* query, index_t* index)
{
/* 
    LOCAL: for in-between OR commands

    create global counter and local counter
    initialize counter
    check for valid query
    strtok the query
    for each word in the query
        if its not "or"
            if its not "and"
                get its counter from index
                add that to local
            if its "and"
                do nothing
        if its "or"
            do something with local
            delete it
*/

    counters_t* currWord = NULL;
    counters_t* local = NULL;
    counters_t* total = NULL;
    counters_t* temp = NULL;

    for (int i = 0; i < query->wordcount; i++) { // loop through every word in the query

        if (strcmp(query->string_tok[i], "or") != 0) { // if this word isn't "or"

            if (strcmp(query->string_tok[i], "and") != 0) { // if this word isn't "and"

                if ((currWord = index_find(index, query->string_tok[i])) != NULL) { // if word is found
                    if (local == NULL) { // if this is the first word after an "or"
                        local = counters_new();
                        query_merge(local, currWord);
                    } else {
                        temp = local;
                        local = query_intersect(local, currWord);
                        counters_delete(temp);
                    }
                } else { 
                    // if word is NOT found, reset local, so that no results will come until an OR happens
                    // this wipes local clean: if it holds nothing, create 
                    // it blank. if it has something, delete and recreate. 
                    if (local == NULL) {
                        local = counters_new();
                    } else {
                        counters_delete(local);
                        local = counters_new();
                    }
                }
            } 
        } else { // word is "or"
            if (total == NULL) { // total doesn't have anything yet
                total = local;
            } else { // total already has values
                query_merge(total, local);
                counters_delete(local);
            }
            local = NULL; // kill local so it can start fresh on the next word

        }


    } // END FOR LOOP

    if (total == NULL && ctr_is_empty(local)) { // nothing found: total is null and local is empty
        counters_delete(local);
        return NULL;
    }
    else if (total == NULL) { // never merged with total -- all "and" operators
        return local;
    }
    else if (ctr_is_empty(local)) { // local is empty but total is not
        counters_delete(local);
        return total;
    }
    else { // both are non-empty. Merge.
        query_merge(total, local);
        counters_delete(local);
        return total;
    }
}

static bool ctr_is_empty(counters_t* ctr) 
{
    int numKeys = 0;
    counters_iterate(ctr, &numKeys, isempty_helper);

    return (numKeys == 0);
}

static void isempty_helper(void* arg, const int key, const int count)
 {
    (*(int*)arg)++;
 }


static void makeDocArray(counters_t* ctr, document_t** docArray, const int docCount) 
{
    if (ctr == NULL || docArray == NULL) {
        fprintf(stderr, "makeDocArray input NULL");
        return;
    }

    counters_iterate(ctr, docArray, putDoc);
}

static void putDoc(void* arg, const int key, const int count)
{
    document_t** docArray = arg;
    int i;
    for (i = 0; docArray[i]->id != 0; i++) {
    }

    docArray[i]->id = key;
    docArray[i]->score = count;
}

/*************** document_new ****************/
/* create empty document structure to populate the initialized 
 * array of documents.
 *
 * needs to free later.
 */
static document_t*
document_new(void) {
	document_t* doc = malloc(sizeof(document_t));
	if (doc!=NULL) {
		doc->id = 0;
		doc->score=0;
	}
	return doc;
}

/************ query_delete ************
* delete function for a query type
*/
static void query_delete(query_t* query)
{
    if (query == NULL) {
        fprintf(stderr, "Null query for deletion");
    }
    mem_free(query->string);
    for (int i = 0; i < query->wordcount; i++) {
        free(query->string_tok[i]);
    }
    mem_free(query->string_tok);
    mem_free(query);
}


