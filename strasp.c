/* Copyright 2003
 * Erick Bourgeois, <erick@jeb.ca>
 * This is free software distributed under the terms of the
 * GNU Public License.
 * 
 * These 4 functions (2 of which are static) comprise strasp.
 * strasp() seperates, or splits, a string according to a string
 * of delimeters and returns an array of pointers to newly
 * allocated strings.
 * 
 * The motivation for this function is to remove string
 * manupulation that occurs with strsep; the original string
 * sent to strasp() (i.e., `str') will not be modified in any
 * way. However, the draw back to this is that the newly
 * allocated array needs to be free'd() (See below).
 * 
 * Note: the array needs to be free'd(). The non-static 
 * strasp_free() function is provided for this reason.
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strasp.h"

static char *rcsid = "$Id: strasp.c,v 1.8 2003/02/17 1:13:34 EST ebourgeois Exp $";
#if 0
#if __GNUC__ == 2
#define USE(var) static void * use_##var = (&use_##var, (void *) &var) 
USE (rcsid);
#endif
#endif

/* This function safely allocates len + 1 bytes
 * and copies len bytes starting from ptr, then
 * it NULL terminates the new string.
 *
 * Precondition: none
 * Return value: a pointer to the NULL terminated 
 *               allocated memory, otherwise NULL.
 *               That is, if len <= 0, or if it 
 *               could not allocate enough memory.
 */
static char *
__strasp_xmalloc(char *str, int len)
{
	char *ptr;
	
	if (len <= 0)
                return NULL;
       
        ptr = (char *) malloc(sizeof(char) * (len + 1));
        if (ptr == NULL)
                return NULL;
	
        ptr = memcpy(ptr, str, len);
        ptr[len] = '\0';
        
        return ptr;
}

/* This function free's() an **array. It is needed if something
 * may go wrong internally and we have already mallocated
 * **array.
 *
 * Precondition: none
 * Postcondition: array is now NULL
 * Return value: none
 */
void
strasp_free(char **array)
{
        unsigned int i;
        
        if (array == NULL)
                return;
        
        for (i = 0; array[i] != NULL; i++)
                free(array[i]);
       
        free(array);
        
        return;
}

/* A general string splitter.
 * Given a string, split it up according to any
 * character in delim, then return an array of
 * these values.
 * 
 * Precondition: none
 * Return value: char ** (array of char *'s)
 * Note: return value should be free'd()
 */
char **
strasp(char *str, const char *delim)
{
	char *head;
	char *tail;
	char *tmp;
	char **array;
	/* array size */
	size_t a_size;
	size_t diff;
	
	if (str == NULL || delim == NULL || strlen(str) == 0)
		return NULL;
		
	head = str;
	/* remove all occurences of any char in delim
	   from the start and the end of str */
	while (*head != '\0' && (strchr(delim, *head++) != NULL))
                ;
        
	head--;
        /* nothing is left in head */
        if (head > str)
                if (strlen(head) == 0)
                        return NULL;

	tmp = head + strlen(head);
        /* remove all occurences of any char in delim
	   from the start and the end of str */
	while (strchr(delim, *--tmp) != NULL)
                ;
        
        /* nothing is left at the end */
        if (head >= ++tmp)
                return NULL;

	/* initiallize */
	tail = tmp;
	tmp = head;
	array = NULL;
	
	/* 1 element since strlen(str) > 0
	   and the other for '\0' */
        a_size = 2;
	
        /* let's get to it; process the supplied string
           how many characters in delim occur in str. */
	while (tmp != tail)
	{		
		if (strchr(delim, *tmp) != NULL)
		{
			diff = tmp - head;
			
			STRASP_ADDELEM(array, a_size);
			STRASP_INSERT_STR(array, a_size, head, diff);
			
			/* move tail along while it is in delim */
			while (*tmp != '\0' && strchr(delim, *++tmp) != NULL)
				;
			
			head = tmp;
			a_size++;
		}
		tmp++;

		if (*tmp == '\0' || tmp == tail)
		{
			diff = tmp - head;
			
			STRASP_ADDELEM(array, a_size);
			STRASP_INSERT_STR(array, a_size, head, diff);
			
			a_size++;
		}
	}

	if (array != NULL || a_size != 2)
		array[a_size - 2] = '\0';
	
	char **tmp_a = array;
	
	return array;
}

