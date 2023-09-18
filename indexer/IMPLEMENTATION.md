# CS50 TSE Indexer
## Implementation Spec

In this document, we discuss the implementation for the `indexer` subsystem of the Tiny Search Engine. This spec is modeled on the Implementation Spec unit in class, as well as the spec for the TSE Crawler. It touches on the core aspects of the implementation spec, such as:
* Data structures
* Control flow: pseudo code for overall flow, and for each of the functions
* Detailed function prototypes and their parameters
* Error handling and recovery
* Testing plan

## Data Structures
The Indexer uses the data structure `index`, the module for which is in the `common` directory. `index` utilizes two major data structures itself: Hashtables and counters. The `index_t` struct has a member `table`, which holds the hashtable that indexes the words. This hashtable maps a `word` to a `counter` object, which holds `(docID, #occurrences)` pairs.

## Control Flow
The Indexer is implemented in one file indexer.c, with four functions:

### main
The main function follows the following protocol:
```
initialize modules
call parseArgs
build the index, save to a file, and delete
exit zero
```

### parseArgs
The parseArgs function parses the command line, extracting the arguments into pageDirectory and indexFileName. The function validates the following criteria:
* Correct number of arguments (3)
* Directory is a valid page directory (does it have a `.crawler` file?)
* indexFileName points to a valid, writable file
* if any errors arise, print to stderr and exit non-zero

### indexBuild
The indexBuild function does the work of building the index from the pageDirectory. Psuedocode (from DESIGN.md):
```
create a new 'index' object
loop over document ID numbers, counting from 1
  load a webpage from the document file 'pageDirectory/id'
  if successful, 
    pass the webpage and docID to indexPage
```

### indexPage
The indexPage function takes a single webpage and adds each non-trivial word to the index, or increments their count if the word is already represented. Pseudocode (from DESIGN.md):
```
step through each word of the webpage,
  skip trivial words (less than length 3),
  normalize the word (converts to lower case),
  look up the word in the index,
    add the word to the index if needed
  increment the count of occurrences of this word in this docID
```

### Other modules
#### pagedir
Along with the `pagedir` functions written for the Crawler subsystem, two more functions were implemented:
 , as well as load a file from a webpage

#### pagedir_validate
`pagedir_validate` validates that a specific directory was a Crawler-produced directory. Pseudocode:
```
create the fileName for the .crawler file
if this file exists and is readable
  return true
if not
  return false
```

`pagedir_readFile` is used to load a webpage from a file to which it is saved. Pseudocode:
```
read the first line as URL
read the second line as epth
read the rest of file as HTML
turn depth into an integer
create a webpage_t using URL, depth, HTML
```

### Function prototypes
#### indexer
The `indexer.c` file has the following functions:
```c
int main(int argc, char* argv[])
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```

### Error handling and recovery
The parseArgs function validates each of the parameters, and if errors arise (noted above), it will print an error message to stderr and exit non-zero. All of the code uses defensive programming techniques, rigorously checking input parameters before beginning other processes.

Out of memory errors are mostly handled by `mem_assert` or other variants, terminating and printing to stderr if there is a memory allocation error.

### Testing plan
#### testing.sh
For testing, I ran a series of tests that first considered edge cases (advised by DESIGN.md): 1. no arguments 2. one argument 3. three or more arguments 4. invalid pageDirectory (non-existent path) 5. invalid pageDirectory (not a crawler directory) 6. invalid indexFile (non-existent path) 7. invalid indexFile (read-only directory) 8. invalid indexFile (existing, read-only file)

I ran indexer on directories from the shared/tse/crawldata folder, and used the shared indexcmp function to confirm that they produced the same output as the answer key indices in the shared folder.

I also ran valgrind on indexer and indextest.