/* 
 * readlinep - a utility function to safely read one line of input,
 * of any length, and return a pointer to a null-terminated string.
 *
 * David Kotz, April 2017, 2019, 2021
 */

#ifndef __READLINEP_H__
#define __READLINEP_H__

#include <stdio.h>

/* readLinep, freadLinep:
 *
 * Read a line from the file into a null-terminated string.
 *   readLinep() is equivalent to freadLinep(stdin).
 * We assume:
 *   caller provides (for freadLinep) a valid FILE open for reading.
 * We return:
 *   a pointer to a malloc'd string; caller must later free() the pointer;
 *   a NULL if EOF or any error.
 * We guarantee:
 *   null-terminated string in the buffer;
 *   no newline in the buffer.
 * Caller is responsible for:
 *   eventually calling free() on the pointer we return.
 * Notes:
 *   If end-of-file is reached before newline, that is not an error.
 *   The string returned contains all characters before that end-of-file.
 */
extern char* readLinep(void);
extern char* freadLinep(FILE *fp);

#endif //  __READLINEP_H__
