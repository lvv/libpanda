/******************************************************************************
  Memory.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      09012001

  Purpose:
    Abstract the out of memory tests so that calling functions can asusme that
    memory was returned by malloc et al.

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

#if defined _WINDOWS
#include "stdafx.h"
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

#include <stdio.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_xmalloc
PURPOSE allocate some memory

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void *panda_xmalloc (size_t size);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Allocate some memory, performing all of the error checking so we don't have to elsewhere.

RETURNS A void * to the memory allocated.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

char *memory;

memory = (char *) panda_xmalloc(42);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void *
panda_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      panda_error (panda_true, "panda_xmalloc failed to allocate memory");
    }

  return buffer;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_xrealloc
PURPOSE allocate some memory, resizing it if already exists

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void *panda_xmalloc (void *now, size_t size);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Allocate some memory, performing all of the error checking so we don't have to elsewhere. Resize the memory if it already exists.

RETURNS A void * to the memory allocated.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

char *memory;

memory = (char *) panda_xrealloc(memory, 42);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void *
panda_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      panda_error (panda_true, "panda_xrealloc failed to allocate memory");
    }

  return buffer;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_xfree
PURPOSE deallocate memory in a safe manner

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_xfree (void *p);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Deallocate a block of memory in a safe manner. For instance, some systems will not allow you to free a NULL pointer...

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

char *p;
p = panda_xmalloc(42);
panda_xrealloc(p);
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_xfree (void *memory)
{
  if (memory != NULL)
    free (memory);
}
