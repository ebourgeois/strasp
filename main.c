/* Copyright 2003
 * Erick Bourgeois, <erick@jeb.ca>
 * This is free software distributed under the terms of the
 * GNU Public License.
 * 
 * This shell like driver is used to demonstrate the strasp function.
 * 
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strasp.h"

static char *rcsid = "$Id: main.c,v 1.2 2003/02/15 14:13:34 EST ebourgeois Exp $";

int
main (int argc, char **argv)
{
	char string[STRASP_ARRBUFSIZ];
	char *str_ptr = string;
	const char *delim = "";
	char **myargv;
	char **tmp_array;
	
	if (argc != 2)
	{
		printf("usage: strasp \"delim\"\n");
		return 1;
	}
	delim = argv[1];
	
	while (1)
	{
		printf("strasp> ");
		if ((str_ptr = fgets(str_ptr, STRASP_ELEBUFSIZ, stdin)) == NULL)
		{
			perror("gets");
			return 1;
		}
		/* split the command line into `words',
		 * delimited by any char in delim */
		str_ptr[strlen(str_ptr)-1] = '\0';
		myargv = strasp(str_ptr, delim);
		
		if (myargv == NULL)
			continue;

		tmp_array = myargv;
		while (tmp_array && *tmp_array)
			printf("%s\n", *(tmp_array++));

		strasp_free(myargv);
	}
	
	return 0;
}

