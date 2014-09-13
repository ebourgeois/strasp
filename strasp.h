/* Copyright (C) 2000
 * Erick Bourgeois, <erick@jeb.ca>
 * This is free software distributed under the terms of the
 * GNU Public License.
 *
 * $Id: strasp.h,v 1.8 2003/02/17 1:13:34 ebourgeois Exp $
 */

#ifndef _STRASP_H
#define _STRASP_H

/* Define a suitable buffer size */
#ifndef BUFSIZ
#  define BUFSIZ 8192
#endif

/* Each element in the array returned by strasp is a maximum
   length of STRASP_ELEBUFSIZ */
#ifndef STRASP_ELEBUFSIZ
#  define STRASP_ELEBUFSIZ BUFSIZ
#endif

/* By default, the array holds 512 pointers/elements */
#ifndef STRASP_ARRBUFSIZ
#  define STRASP_ARRBUFSIZ (STRASP_ELEBUFSIZ >> 4)
#endif

#define __STRASP_ADDELEM(array, a_size) \
	if (a_size > STRASP_ARRBUFSIZ) \
	{ \
		array[a_size - 2] = '\0'; \
		return array; \
	} \
	array = (char **) realloc(array, a_size * sizeof(char *)); \
	if (array == NULL) \
	{ \
		array[a_size - 2] = '\0'; \
		return array; \
	}
#define STRASP_ADDELEM __STRASP_ADDELEM
	
#define __STRASP_INSERT_STR(array, a_size, str, len) \
	if (len > STRASP_ELEBUFSIZ) \
	{ \
		array[a_size - 2] = '\0'; \
		return array; \
	} \
	array[a_size - 2] = __strasp_xmalloc(str, len); \
	if (array[a_size - 2] == NULL) \
	{ \
		array[a_size - 2] = '\0'; \
		return array; \
	}
#define STRASP_INSERT_STR __STRASP_INSERT_STR
	
/* Provide this as a convenience free'n function */
void strasp_free (char **array);
char **strasp (char *str, const char *delim);

#endif /* !_STRASP_H */

