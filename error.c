/******************************************************************************
  Error.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda can experience errors for many reasons. This file contains
    centralised error handling code to make the programming easier and more
    consistent elsewhere. The code should also handle all the platforms
    the code is expected to run on correctly.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined _WINDOWS
  #include "panda/constants.h"
  #include "panda/functions.h"

  #include <windows.h>
#else
  #include <panda/constants.h>
  #include <panda/functions.h>
#endif

/******************************************************************************
DOCBOOK START

FUNCTION panda_error
PURPOSE handle a <command>Panda</command> error

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_error (int fatal, char *msg);
SYNOPSIS END

DESCRIPTION Print out the error message to stderr and then exit (if fatal is panda_true).

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_error ("All is rotten in the state of Denmark");
EXAMPLE END
DOCBOOK END
******************************************************************************/

// You can bang or head or you can drown in a hole
//                                                    -- Vanessa Amarosi, Shine
void
panda_error (int fatal, char *message)
{
#if defined _WINDOWS
  MessageBox(NULL, message, 
	  (fatal == panda_true) ? "Fatal Panda Windows DLL error" :
                              "Non-fatal Panda Windows DLL error", MB_OK);
#else
  fprintf (stderr, "%s\n", message);
#endif

  if(fatal == panda_true) exit (42);
}
