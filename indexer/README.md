# CS50 TSE Indexer
## Nikhil Pande

The `indexer` subsystem of the Tiny Search Engine takes a directory of files, produced by a crawler, and stores every word in an index. It utilizes a new `index` data structure, which is mostly a wrapper for `hashtable` to do this.

Much of the usage and testing information is covered in IMPLEMENTATION.md

### Compilation
To compile, run `make`. Run `make indexer` to compile `indexer` and `make indextest` to compile `indextest`. To run `testing.sh` and save it to an output file `testing.out`, call `make test`. To use `indexer` in the command line, use the following format: 
`./indexer pageDirectory indexFilename`

### Files
Indexer includes the following files:
`Makefile` - compilation procedure
`IMPLEMENTATION.md` - implementation documentation
`indexer.c` - implementation
`indextest.c` - index testing
`testing.sh` - bash test file
`testing.out` - testing output

### Assumptions
We assume that the file to read are in the format of
* URL \n
* depth \n
* HTML

We also assume that the `oldIndexFilename` for indextest will be in the form: `word [docID1 count1] ... [docIDn countn]`

### NOTE (error):
The results for my valgrind for indextest say that there is one block of memory leak that is still reachable. It notes that the leak is in `index_loadIndex`, a function in `index.c` that creates an `index_t` struct from an index file. It says the problem is with `fopen`, and initially I checked and realized I had never called `fclose` on `FILE* fp`, but when I fixed that, it still showed a leak, which is confusing. Either way, I wanted to note this memory leak in this document.
