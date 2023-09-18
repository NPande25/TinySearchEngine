/*
* hashtable.c implements the hashtable module
*
* Nikhil Pande
* COSC 50, 23S
* Professor Palmer
*
* See "hashtable.h" for more info
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "hashtable.h"
#include "set.h"
#include "hash.h"

/****** global types *******/
typedef struct hashtable {
  set_t** table;  //creates the array
  int slots;
} hashtable_t;

/******* gloabal functions ********/
/* Create a new hashtable. See "hashtable.h" for info. */
hashtable_t* 
hashtable_new(const int num_slots)
{   
    if (num_slots > 0) {
        hashtable_t* ht = mem_malloc(sizeof(hashtable_t));
    

        if (ht != NULL) { // allocation worked and valid num_slots
            ht->table = calloc(num_slots, sizeof(set_t*));  // allocate memory for one set per table slot
            ht->slots = num_slots;
            
            if (ht->table != NULL) { // table memory worked
                for (int i = 0; i < num_slots; i++) {
                    ht->table[i] = set_new(); // initialize new set for each element in table
                }
                return ht;
            }
            return NULL; // if table memory didn't allocate
        }
        return NULL; // if NULL ht
    }
    return NULL; // if invalid num_slots
}

/* Insert an item into the hashtable. See "hashtable.h" for info. */
bool 
hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
    if (ht != NULL && key != NULL && item != NULL) {
        int slot = hash_jenkins(key, ht->slots); // calculate hash value via Jenkins function
        return set_insert(ht->table[slot], key, item); // insert key into set at specified slot
    }
    return false;
}

/* Find the value at the given key. See "hashtable.h" for info. */
void* 
hashtable_find(hashtable_t* ht, const char* key) {
    if (ht != NULL && key != NULL) {
        int slot = hash_jenkins(key, ht->slots); // calculate hash
        return set_find(ht->table[slot], key);
    }
    return NULL;
}

/* Prints the hashtable. See "hashtable.h" for info. */
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item)) {
    if (fp != NULL) {
        if (ht != NULL) {
            for (int i = 0; i < ht -> slots; i++){
                if (itemprint != NULL) {

                    fprintf(fp, "Slot %d: ", i); // print which slot we're in
                    set_print(ht -> table[i], fp, (*itemprint));
                    fprintf(fp, "\n"); // new line

                } else { // null itemprint
                    fprintf(fp, "Slot %d:\n", i); // print which slot we're in
                }
            }
        } else {
            fprintf(fp, "(null)");
        }
    }   
}

/* Iterate through the hashtable. See "hashtable.h" for info. */
void 
hashtable_iterate(hashtable_t* ht, void* arg, 
                            void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (ht != NULL && itemfunc != NULL) { // check input conditions

        for (int i = 0; i < ht->slots; i++) { // iterate over slots
            set_iterate(ht->table[i], arg, (*itemfunc)); // iterate over set in each slot, performing itemfunc
        }
    }
}

/* Delete hashtable, freeing all memory allocations. See "hashtable.h" info. */
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) )
{
    if (ht != NULL) {
        for (int i = 0; i < ht->slots; i++) {
            set_delete(ht->table[i], (*itemdelete));
        }
        mem_free(ht -> table);  // frees memory for table
        mem_free(ht);
    }
}