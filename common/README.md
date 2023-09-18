# Common Library
# CS50 Tiny Search Engine

## pagedir
### Usage
The `pagedir.c` file is used to initialize a directory and save crawled webpages to that directory. It implements two functions:
```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* pagedir_readFile(FILE* fp);
bool pagedir_validate(char* pageDirectory);
```
`pagedir_init` initializes the pageDirectory. This is where we will store the documented web pages.

`pagedir_save` saves the webpages to the aforementioned directory by writing its contents into a file

`pagedir_readFile` loads a webpage from a file to which it is saved.

`pagedir_validate` validates that a specific directory was a Crawler-produced directory.

## index
### Usage
The `index.c` file implements the index data structure, on which the indexer subsystem relies. The `index_t` struct holds a hashtable that maps words to (docID, #occurrences) pairs, using the counter data structure. `index.h` exports the following functions:
```c
index_t* index_new(const int numSlots);
bool index_insert(index_t* index, const char* word, const int ctr_key);
bool index_save(index_t* index, char* fileName);
void index_print(index_t* index, FILE* fp);
bool index_delete(index_t* index);
```
`index_new` creates a new index structure

`index_insert` inserts a word at a certain counter key into the index.

`index_save` saves the index into a file, aided by helper functions `save_table` and `save_counter`. The function saves the index in the format `word docID count [docID count]...`

`index_print` is a function I used for debugging, printing each word in the index at its specified slot

`index_delete` deletes the index, calling hashtable_delete and freeing the index from memory

## query
### Usage
The `query.c` file implements the library functions for queries, which involve merging and intersecting two countersets for the `querier` subsystem. `query.h` exports the following functions:
```c
void query_merge(counters_t* c1, counters_t* c2);
counters_t* query_intersect(counters_t* c1, counters_t* c2);
int query_count(counters_t* ctr);
```
`query_merge` merges two countersets, overwriting `c1` with both counters' data

`query_intersect` looks for common keys in two counters and inserts them into a third counterset, which it returns

`query_count` counts the amount of keys in the counterset

## Files
* `Makefile` - library compilation
* `README.md` - documentation
* `pagedir.c` - implements the page directory functions
* `pagedir.h` - header file
* `index.c` - implements index data structure
* `index.h` - header file
* `word.c`- implements word functions
* `word.h` - header file
* `query.c` - implements query functions
* `query.h` - header file

## Assumptions
No assumptions beyond those in the specs. All input assumptions are documented in `pagedir.h`, `index.h`, `word.h`, `query.h`.


