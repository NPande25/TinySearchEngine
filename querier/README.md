# CS50 TSE Querier
## Nikhil Pande

The `querier` subsystem is the third system in the Tiny Search Engine system. For this subsystem, I implemented the `querier.c` program, and I implemented further capabilites in the `../common/index.c` module. The querier takes in a `PageDirectory` and an`indexFileName`, then returns the relevant query based on the inputed search words. The usage is as follows:
    `$ /.indexer <pageDirectory> <indexFilename>

## Files
we utilize the following files for the querier implmeentation:
* Makefile
* README.md
* IMPLEMENTATION.md
* DESIGN.md
* querier.c
* testing.sh
* testing.c

## Limiatations:
My implementation does not support multiple spaces at the end of the word. In this case, there will be a segmentation fault.
