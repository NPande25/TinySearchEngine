/*
*
* set.c provides a 'set' module
* similar to the 'bag' module in bag.c
*
* Nikhil Pande
* CS50 23S, Professor Palmer
* Lab 3
* 
* see set.h for detailed function descriptions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include <stdbool.h>
#include "mem.h"

/***** local types ******/
typedef struct setnode {
    void* item;
    struct setnode* next; // connect it to the next node
    char* key;
} setnode_t;

/****** global types ******/
typedef struct set {
    struct setnode* head;
} set_t;

/******* local functions *******/
/* setnode_new creates a new set node and returns it. Note: static function, not visible outside file */
static setnode_t* setnode_new(char *key, void *item) {
    
    setnode_t* node = mem_malloc(sizeof(setnode_t));

    if (node != NULL) {     // if allocation worked...

        node->item = item;  // initialize node and pointers
        node->key = key;
        node->next = NULL; 
        return node;

    } else {                // allocation failed, return null
        return NULL;
    }
}

void* set_find(set_t* set, const char* key); // add function declaration before set_insert()


/********** Global functions ***********/
/* Create a new set. See "set.h" for info. */
set_t* set_new(void)
{
    set_t* set = mem_malloc(sizeof(set_t)); // allocate the memory

    if (set == NULL) {                      // if didn't work, no set --> error
        return NULL;
    } else {
        set->head = NULL;                   // initialize head
        return set;
    }
}

/* Insert a node into the set. See "set.h" for info. */
bool set_insert(set_t* set, const char* key, void* item)
{   
    if (set!= NULL && key != NULL && item != NULL) {

        // copy key into char without constant qualifier
        char *key2 = mem_malloc(strlen(key) + 1);
        strcpy(key2, key);

        if (set_find(set, key) == NULL) { // if the key is not already in the set
            setnode_t *node = setnode_new(key2, item);

            node->next = set->head;
            set->head = node;

            return true;
        }
        else { // else free the key
            mem_free(key2);
        }
    }

    return false;
}

/* Find a node in the set based on a key. See "set.h" for info. */
void* set_find(set_t* set, const char* key) 
{
    if (set != NULL && key != NULL) {
        for(setnode_t *node = set->head; node != NULL; node = node->next) {
            if (strcmp(key, node->key) == 0) { // if the key matches the node's key
                return node->item;
            }
        }
    }
    return NULL;
}

/* Print the set. See "set.h" for info. */
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) )
{    if (fp != NULL) {
        if (set != NULL) {
            fputc('{', fp);
            for (setnode_t* node = set->head; node != NULL; node = node->next) { // iterate over set
                if (itemprint != NULL) {
                    (*itemprint)(fp, node->key, node->item);

                    if (node->next != NULL) {
                        fputc(',', fp); // separate by comma
                    }
                }
            }
            fputc('}', fp);           // close bracket and new line
            fputs("\n", fp);


        } else { // if null set
            fputs("(NULL)\n", fp);
        }
    }
}

/* Iterate over set and call (*itemfunc), which is passed in. See "set.h" for info. */
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (set != NULL && itemfunc != NULL) {
        for (setnode_t* node = set->head; node != NULL; node = node->next) { // iterate over set
            (*itemfunc)(arg, node->key, node->item);            // call itemfunc
        }
    }
}

/* Delete the set, freeing all memory allocations. See "set.h" for info. */
void set_delete(set_t* set, void (*itemdelete)(void* item) )
{
    if (set != NULL) {
        for (setnode_t* node = set->head; node != NULL; ) {
            
            if (itemdelete != NULL) {
                (*itemdelete)(node->item);
            }

            setnode_t* next = node->next; // store next before we free next
            mem_free(node->key);        // free key
            mem_free(node);
            node = next;
        }

        mem_free(set);
    }
}
