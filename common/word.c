
/*
 * Nikhil Pande
 * COSC 50, 23S
 * Tiny Search Engine
 *
 * word.c - a Word module to normalize words in the indexer
 */


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


// normalizeString | converts a string into lower case 

bool normalizeString(char* str)
{
    if(str == NULL ) {
        return false;
    }
    // turn each character to lower
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
    return true;
}