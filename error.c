/******************************************************************************
  Error.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda can experience errors for many reasons. This file contains
    centralised error handling code to make the programming easier and more
    consistent elsewhere. The code should also handle all the platforms
    the code is expected to run on correctly.

  Copyright (C) Michael Still 2000 - 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined _WINDOWS
#include "stdafx.h"
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
    MessageBox (NULL, message, 
		(fatal ==
		 panda_true) ? "Fatal Panda Windows DLL error" :
		"Non-fatal Panda Windows DLL error", MB_OK);
  
#else
    fprintf (stderr, "%sError: %s\n", (fatal == panda_true) ? "Fatal " : "",
	     message);
#endif /*  */
  if (fatal == panda_true)
    exit (42);
}
