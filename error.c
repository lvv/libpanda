/******************************************************************************
	Error.c

	Change Control:													DDMMYYYY
		Michael Still		File created							03062000

	Purpose:
		Panda can experience panda_errors for many reasons. This file contains
		centralised panda_error handling code to make the programming easier and more
		consistent elsewhere. The code should also handle all the platforms
		the code is expected to run on correctly.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <panda/functions.h>

/******************************************************************************
	Handle an panda_error.
******************************************************************************/

void
panda_error (char *message)
{
  fprintf (stderr, "%s\n", message);
  exit (42);
}
