/******************************************************************************
  internal.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      18012001

  Purpose:
    These are calls that are internal, but more generally useful than those
    found in some of the functionality specific code files...

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

/******************************************************************************
DOCBOOK START

FUNCTION panda_entergraphicsmode
PURPOSE make sure the page description stream is in graphics mode

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_entergraphicsmode (panda_page * target);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. PDF pages are described with a text stream full of commands. Many of these commands are similar to those used within Postscript. This function call ensures that the text stream is in a graphics state.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
panda_entergraphicsmode (page);
EXAMPLE END
SEEALSO panda_exitgraphicsmode
DOCBOOK END
******************************************************************************/

void
panda_entergraphicsmode (panda_page * target)
{
  //  - save the current graphics state (q operator, p 386 of spec)
  //  - setup the current transformation matrix (ctm, s 3.2 and p 323 of spec)
  //    such that the image is scaled correctly (cm operator)
  //  - modify the ctm to shift the image to where it is meant to be on the
  //    the page
  //  - use the image xobject we have created (Do operator, p 348 of spec)
  //  - restore the graphics state to the way it was previously (Q operator,
  //    p 386 of spec)
  if (target->contents->textmode == panda_true)
    {
      target->contents->layoutstream =
	panda_streamprintf (target->contents->layoutstream, "ET\nq\n");
      target->contents->textmode = panda_false;

#if defined DEBUG
      printf ("Ended textmode for object %d\n", target->contents->number);
#endif
    }
  else if (target->contents->insidegraphicsblock == panda_false)
    {
      target->contents->layoutstream =
	panda_streamprintf (target->contents->layoutstream, "q\n");
      target->contents->insidegraphicsblock = panda_true;
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_exitgraphicsmode
PURPOSE make sure the page description stream is out of graphics mode

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_exitgraphicsmode (panda_page * target);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. PDF pages are described with a text stream full of commands. Many of these commands are similar to those used within Postscript. This function call ensures that the text stream is not in a graphics state.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
panda_entergraphicsmode (page);
...
panda_exitgraphicsmode (page);
EXAMPLE END
SEEALSO panda_entergraphicsmode
DOCBOOK END
******************************************************************************/

void
panda_exitgraphicsmode (panda_page * target)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "Q\n\n");
  target->contents->insidegraphicsblock = panda_false;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_createandinsertpage
PURPOSE create a page within the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_createandinsertpage (panda_pdf *output);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function creates the objects required for a page to exist within <command>Panda</command>. This function is wrappered by other <command>Panda</command> function calls.

RETURNS A pointer to a panda_page object

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_createandinsertpage(document);
EXAMPLE END
DOCBOOK END
******************************************************************************/

panda_page *
panda_createandinsertpage (panda_pdf * output)
{
  panda_page *newPage;
  panda_pagelist *lastPage, *prevPage;

  // Make some space for the object
  newPage = (panda_page *) panda_xmalloc (sizeof (panda_page));

  // Store the page in the pagelist
  lastPage = output->pageholders;
  while (lastPage->next != NULL)
    {
      lastPage = lastPage->next;
    }

  // Add the page to the list
  lastPage->me = newPage;
  lastPage->next = panda_xmalloc (sizeof (panda_pagelist));
  lastPage->next->next = NULL;

  // Make the new page object
  newPage->obj = (panda_object *) panda_newobject (output, panda_normal);

  // Add it to the object tree
  panda_addchild (output->pages, newPage->obj);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  newPage->contents = (panda_object *) panda_newobject (output, panda_normal);
  panda_addchild (newPage->obj, newPage->contents);
  panda_adddictitem (output, newPage->obj, "Contents", panda_objectvalue,
		     newPage->contents);
  newPage->contents->isContents = panda_true;

  // Setup some stuff in the contents object that we need later
  newPage->contents->textmode = panda_false;
  newPage->contents->insidegraphicsblock = panda_false;

  // Increment the page count for the PDF
  output->pageCount++;

  return newPage;
}
