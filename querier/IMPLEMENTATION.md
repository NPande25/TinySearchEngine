# CS50 TSE Indexer
## Implementation Spec

In this document, we discuss the implementation of the `querier` subsystem, which is mostly contained in the `querier.c` file. This document details the following elements:
* Data structures
* Control flow: pseudo code for overall flow, and for each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan

### Data Structures
The `querier` system utilized several data structures in its implementation, some of which have been created in previous labs, and some that were implemented for this lab only. These include:
* `query_t`
* `document_t`
* `index_t`
* `counters_t`

#### query
The `query_t` data structure is used to hold the query as a string, an array with the normalized and tokenized query, and the wordcount as an integer.

#### document
The `document_t` data structure is used to hold information about the documents that matched the queried words, storing their URL information so it can be printed in the list

### Control Flow
This section describes the functions that I used for the `querier.c` program.

####
```c
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFileName);
```
We first begin in the main function, as we parse the arguments to validate them and extract them from the function parameters. We check the number of arguments, the validity of the `pageDirectory` by using `pagedir_validate`, and the `indexFileName` for an existing path name.
```c
static char* normalize_query(query_t* query);
```
Then, we have a function that normalizes the inputted query: This transforms all letters to lowercase, ignores non-alphabetic characters, and trims duplicate spaces. If there are any non-alphabetic or non-space characters found in the query, the function prints to stderr and exits non-zero.
```c
static void tokenize_query(query_t* query);
```
The query can then be tokenized by the `tokenize_query` function. In this function, we take the next word of the query (via the `getNextWord` function), and insert it into an array of strings, which is stored back in the `query` type.
```c
static void querier(char* pageDirectory, char* indexFilename);
```
The querier function takes control of the interface for the querier. It initializes the necessary structures and solicits user input until the user-specified end-of-file. When it receives an input, it normalizes, tokenizes, and scores the query. It then begins the presentation, collecting the documents into an array and printing them out to the user.
```
querier:
defense on args
load index from indexFileName

while (readline) until EOF
    initialize query structure
    normalize passed-in query
    tokenize query
    parse and score query, return a counter
    count how many results there are
    create/initialize an array of document types, size = count above
    add documents, put in the URL
    delete result counter
    print the documents
```
```c
static char* getNextWord(const char* str, int* pos);
```
The `getNextWord` function was adapted from webpage_getNextWord, allowing us to easily create the tokenized array of words. With this array, we simply loop through the indices for scoring.
```c
static bool validate_query(query_t* query);
```
The query also must be validated for the specifications in REQUIREMENTS.md, which prevent:
* a leading or trailing logical operator
* consecutive logical operators
We use this function's boolean return value to determine whether the query satisfies the requirements. If an error is found, we print to stderr and return false.
```c
static counters_t* scoreQuery(query_t* query, index_t* index);
```
Scoring the query involves the most complicated algorithm due to specific requirements for the logical operators and the scores. This function takes in an index that it consults for word scores, as well as the query type from which it can read the tokenized query. It loops through that list and, for every new word, determines whether it is a) a regular word or b) a logical operator. Depending on its status, it performs different functions. Overall, this function tracks two countersets: a `total` one and a `local` one. For `total`, it gets updated whenever an OR appears, or at the end of the query. For `local`, its value is updated for every consecutive word, separated by an AND or nothing at all. Of course, the two operators are essentially identical.
```
LOCAL: for in-between OR commands
GLOBAL: the overall counter for the entire query

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
    after the query, combine local/total
    return
```

### Error Handling and Recovery
The parseArgs function validates each of the parameters, and if errors arise (noted above), it will print an error message to stderr and exit non-zero. All of the code uses defensive programming techniques, rigorously checking input parameters before beginning other processes.

Out of memory errors are mostly handled by `mem_assert` or other variants, terminating and printing to stderr if there is a memory allocation error.

### Testing plan
For testing, I first ran a series of tests that considered edge cases such as: 1. no arguments 2. one argument 3. three or more arguments 4. invalid pageDirectory (non-existent path) 5. invalid pageDirectory (not a crawler directory) 6. invalid indexFile (non-existent path).

I also created a document with invalid queries, such as: 1. invalid character 2. leading with logical operator 3. ending with logical operator 4. starting and ending with one 5. having two logical operators consecutively 6. all logical operators. In every case, the program recognized the case and skipped it.

Then, I ran the querier on directories and index files from the `shared/tse/output` folder, and created files in my testing document that had different test cases. I tested the system with differing capitalization, multiple