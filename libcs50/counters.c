/*
* counters module that creates a set of counters,
* each distinguished by an integer key
*
* Nikhil Pande
* CS50, 23S
* Professor Palmer
*
* see "counters.h" for more info
*/

/* include files */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mem.h"
#include "counters.h"


/******* local type ********/
typedef struct counter {
    int key;
    int count;
    struct counter* next; // need to know next counter
} counter_t;

/******* global type *******/
typedef struct counters {
    counter_t* head;
} counters_t;

/******* local functions *******/
/* Local function to create new counter */
static counter_t* new_counter(int key) 
{
    counter_t* counter = mem_malloc(sizeof(counter_t));

    if (counter != NULL) { // if mem_alloc worked
        counter->key = key;
        counter->count = 0;
        return counter;
    } else { // mem_malloc error
        return NULL;
    }
}

int counters_get(counters_t* ctrs, const int key); // declare counters_get and _set so we can use in counters_add
bool  counters_set(counters_t* ctrs, const int key, const int count);

/******* exported global functions *******/
/* Create new counters type. See "counters.h" for info. */
counters_t* 
counters_new(void) 
{
    counters_t* counters = mem_malloc(sizeof(counters_t));

    if (counters != NULL) { // if mem_alloc worked
        counters->head = NULL;
        return counters;
    } else { // malloc error
        return NULL;
    }
}

/* Add new counter if key isn't taken. Increment otherwise. See "counters.h" for more info. */
int 
counters_add(counters_t* ctrs, const int key) {
    if (ctrs != NULL && key >= 0) {
        if (counters_get(ctrs, key) == 0) { // counter not present
            counter_t* new = new_counter(key); // create new

            if (new != NULL) {
                new->count = 1; // count is 1

                new->next = ctrs->head; // add new counter to head
                ctrs->head = new;
                return new->count;

            } else { // error with new counter
                return 0;
            }   
        } else { // counter already there
            int count = counters_get(ctrs, key); // get value
            count++; // increment
            counters_set(ctrs, key, count); // set value at that key to incremented value
            return count;
        }
    }
    
    return 0;
}

/* Get value of counter at specific key. See "counters.h" for more info. */
int 
counters_get(counters_t* ctrs, const int key)
{
    if (ctrs != NULL && key >= 0) {
        // iterate over all counters
        for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) {
            if (counter->key == key) { // if find matching key
                return counter->count; // return its count
            }
         }
    }
    return 0; // if nothing found, ctrs is NULL, or key<0, return 0
}

/* Set counter value at specific key. See "counters.h" for more info. */
bool 
counters_set(counters_t* ctrs, const int key, const int count) {
    if (ctrs != NULL && key >= 0 && count >= 0) {
        // iterate over all counters
        for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) {
            if (counter->key == key) { // if find matching key
                counter->count = count; // set that counter's count
                return true; // success
            }
        }
        // if we didn't find counter and return, add and set it
        counters_add(ctrs, key);
        counters_set(ctrs, key, count);
        return true;
    }
    return false; // if conditions not met
}

/* Print out all the counters. See "counters.h" for more info. */
void 
counters_print(counters_t* ctrs, FILE* fp) {
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);

            // iterate though counters
            for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) { 
                fprintf(fp, "%d:%d", counter->key, counter->count);
                
                if (counter->next != NULL) { // comma separator
                    fputc(',', fp);
                }
            }
            fputc('}', fp);
            fputs("\n", fp);

        } else { // if null ctrs
            fputs("(null)\n", fp);
        }
    }
}

/* Iterate through counters and perform (*itemfunc) function. See "counters.h" for more info. */
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count))
{
    if (ctrs != NULL && itemfunc != NULL) {
        for (counter_t* counter = ctrs->head; counter != NULL; counter = counter->next) { // iterate over counters
            (*itemfunc)(arg, counter->key, counter->count); // perform function
        }
    }
}


/* Delete the counter and all the counters. See "counters.h" for more info. */
void counters_delete(counters_t* ctrs) 
{
    if (ctrs != NULL) {
        for (counter_t* counter = ctrs->head; counter != NULL; ) { 
            counter_t *next = counter->next; // track next
            mem_free(counter);
            counter = next; // iterate over counters
        }
        mem_free(ctrs);
    }
}