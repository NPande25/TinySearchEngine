/* Nikhil Pande
* COSC 50, 23S
* Tiny Search Engine
*
* query.h - header file for query.c
* 
*
*/

#include "counters.h"

/********** query_merge ***********/
/*
* merges all of the keys in the two counters
* 
* inputs:
*   c1 and c2 (counters_t*): the two counters to merge
* outputs:
*   this function combines the two counters. After its call,
*   c1 will have all of the data from c2, along with its old data
*/
void query_merge(counters_t* c1, counters_t* c2);

/********** query_intersect ***********/
/*
* creates a new counterset with the keys present in both c1 and c2.
* It sets the count for the minimum count between c1 and c2.
* 
* inputs:
*   c1 and c2 (counters_t*): the two counters to intersect
* outputs:
*   this function returns a counterset with counters present
*   in both c1 and c2
*/
counters_t* query_intersect(counters_t* c1, counters_t* c2);

/********** query_count ***********/
/*
* uses a helper function to count the number of keys in the counterset
*
* inputs:
*   ctr (counters_t*): counter to count
* outputs:
*   returns the count as an integer
*/
int query_count(counters_t* ctr);




