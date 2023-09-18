/* Nikhil Pande
* COSC 50, 23S
* Tiny Search Engine
*
* query.c - file to implement library functions for querier
* many of the merge/intersect functions are implemented here
* see query.h for more details
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <counters.h>
#include <mem.h>

/**************** local types ****************/
typedef struct ctr_bag {
    counters_t* ctr1;
    counters_t* ctr2;
} ctr_bag_t;

static void query_merge_helper(void* arg, const int key, const int count);
static void query_intersect_helper(void* arg, const int key, const int count);
static void countfunct(void* arg, const int key, const int count);


/* merge two counters*/
void query_merge(counters_t* c1, counters_t* c2)
{
    if (c1 != NULL && c2 != NULL) { // defense
        counters_iterate(c2, c1, query_merge_helper);
    }
}

/* static helper function for merge 
* searches for the current key in the other counterset.
* if not present, create it with the current count
* if present, combine the two counts
*/
static void query_merge_helper(void* arg, const int key, const int count)
{
  if (arg != NULL && count > 0 && key >0) { 
    counters_t* c1 = arg;
    int count2 = count;

    // find the same key in ctrA
    int count1 = counters_get(c1, key);
    if (count1 == 0) {
      // if not found in c1, insert
        counters_set(c1, key, count2);
    } else {
      // if found, add value
        count1 += count2; 
        counters_set(c1, key, count1);
    }  
  }
}

/**************** query_intersect ****************/
/* intersect two counters. see query.h for details. */
counters_t* query_intersect(counters_t* c1, counters_t* c2)
{
    if (c1 == NULL || c2 == NULL) { // defense
        return NULL;
    }
    
    // intialize struct to hold two bags
    ctr_bag_t* ctrBag;
    ctrBag = mem_assert(malloc(sizeof(ctr_bag_t)), "ctrBag");


    ctrBag->ctr1 = c1;
    counters_t* intersect = counters_new();    // need to free this later
    ctrBag->ctr2 = intersect; // create new 
    
    // iterate the bagB, 
    counters_iterate(c2, ctrBag, query_intersect_helper);
    free(ctrBag);

  return intersect;
}

/* static helper function for intersect
 *
 * used as we iterate over c2. For every key that is also
 * present in c1, we add it to intersect with the minimum count.
 * if the key isn't in c1, do nothing
 */
static void query_intersect_helper(void* arg, const int key, const int count)
{
   if (arg != NULL && count > 0 && key > 0) { // defense

       ctr_bag_t* ctrBag = arg;
       int countB = count;

       // find the same key in ctrA
       int countA = counters_get(ctrBag->ctr1, key);
       if (countA != 0) {
        // if found, add existing count and add to the intersect counterset
        if(countA <= countB) {
            counters_set(ctrBag->ctr2, key, countA);
        } else {
            counters_set(ctrBag->ctr2, key, countB);
        }
      }  
    }
}



/* ***************** query_count  ******************* */
/* count number of keys in counterset */
int query_count(counters_t* ctr) {
    int keynum = 0;
    counters_iterate(ctr, &keynum, countfunct);

    return keynum;
}

/* ***************** countfunct  ******************* */
/* counting helper function to count non-null items */
static void countfunct(void* arg, const int key, const int count)
{
    int* item = arg;

    if (item != NULL && key > 0 && count > 0) {
        (*item)++;
    }
}
