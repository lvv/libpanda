/******************************************************************************
  Panda.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000

  Purpose:
    Library level calls. These are the calls normally accessible to the
    user, and these calls wrapper our calls.

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

#define  GLOBALS	here

#if defined _WINDOWS
#include "stdafx.h"
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

/******************************************************************************
DOCBOOK START

FUNCTION panda_init
PURPOSE setup Panda ready for use

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_init (void);
SYNOPSIS END

DESCRIPTION Performs some simple setup of Panda before it is used for the first time in your application.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_init();
EXAMPLE END
DOCBOOK END
******************************************************************************/

// Initialise the PDF library ready for operations
void
panda_init ()
{
  int generalCounter;

  // We first need to create the binary string to include in our header
  for (generalCounter = 0; generalCounter <
       (sizeof (panda_binaryheaderstring) / sizeof (char)); generalCounter++)
    {
      panda_binaryheaderstring[generalCounter] =
	panda_binarychar (panda_headerstring[generalCounter]);
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_open
PURPOSE open a PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
panda_pdf * panda_open (char *filename, char *mode);
SYNOPSIS END

DESCRIPTION Open the named PDF document with the mode specified. The only mode currently supported is "w", but others will be integrated later. The interface to this function is identical in it's behaviour to the <command>fopen</command>() function call offered by the ANSI C standard IO library.

RETURNS A pointer to a panda_pdf structure

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
EXAMPLE END
SEEALSO panda_init, panda_open_actual, panda_open_suppress, panda_close
DOCBOOK END
******************************************************************************/

panda_pdf *
panda_open (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, panda_false);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_open_suppress
PURPOSE open a PDF document with some special options

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
panda_pdf * panda_open_suppress (char *filename, char *mode);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Open the named PDF document with the mode specified. The only mode currently supported is "w", but others will be integrated later. The interface to this function is identical in it's behaviour to the <command>fopen</command>() function call offered by the ANSI C standard IO library. This function call additionally doesn't create any of the default objects that a panda_open would create, which is useful when you want to parse an existing PDF into a new document.

RETURNS A pointer to a panda_pdf structure

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open_suppress("filename.pdf", "w");
EXAMPLE END
SEEALSO panda_init, panda_open_actual, panda_open, panda_close
DOCBOOK END
******************************************************************************/

panda_pdf *
panda_open_suppress (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, panda_true);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_open_actual
PURPOSE actually open a PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
panda_pdf * panda_open_actual (char *filename, char *mode);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Open the named PDF document with the mode specified. The only mode currently supported is "w", but others will be integrated later. The interface to this function is identical in it's behaviour to the <command>fopen</command>() function call offered by the ANSI C standard IO library. This call actually does the opening for <command/>panda_open</command>() and <command>panda_open_suppress</command>().

RETURNS A pointer to a panda_pdf structure

EXAMPLE START
Use panda_open!
EXAMPLE END
SEEALSO panda_init, panda_open, panda_open_suppress, panda_close
DOCBOOK END
*****************************************************************************/

panda_pdf *
panda_open_actual (char *filename, char *mode, int suppress)
{
  panda_pdf *openedpdf;
  char *tempPtr, *newmode, *verStr;

  // We are going to open a PDF for file I/O. Currently, the only supported
  // mode is 'w'. There are some more obscure modes not included in the error
  // checking below (like a+b), deal with it.

  // Make some space for the PDF information
  openedpdf = (panda_pdf *) panda_xmalloc (sizeof (panda_pdf));

  // Every PDF needs a database
  panda_dbopen (openedpdf);

  // Every PDF is going to have to have some xref information associated with
  // it at some stage.
  openedpdf->xrefList = (panda_xref *) panda_xmalloc (sizeof (panda_xref));
  openedpdf->xrefList->next = NULL;
  openedpdf->xrefTail = openedpdf->xrefList;

  // Do stuff based on the mode of the opening
  switch (mode[0])
    {
    case 'r':
    case 'a':
      panda_error (panda_true, "Unsupported file I/O mode handed to panda.");
      break;

    case 'w':
      // We are opening a new PDF for writing (we hope)
      if (strlen (mode) > 1)
	switch (mode[1])
	  {
	  case '+':
	    panda_error (panda_true,
			 "Unsupported file I/O mode handed to panda.");
	    break;
	  }

      // Opening the file with the binary option makes Windows work
      newmode = panda_xsnprintf ("%cb", mode[0]);

      // We _are_ going to create the file
      if (strcmp (filename, "-") == 0)
	openedpdf->file = stdout;
      else if ((openedpdf->file = fopen (filename, newmode)) == NULL)
	return NULL;

      // We have no objects yet
      openedpdf->nextObjectNumber = 1;
      openedpdf->nextPHObjectNumber = 1;
      openedpdf->totalObjectNumber = 0;

      // We are at the begining of the file
      openedpdf->byteOffset = 0;

      // This stuff is avoided by the lexer
      if (suppress == panda_false)
	{
	  // The file will need to have a PDF header to it
	  panda_printf (openedpdf, "%s%s\n", panda_magicheaderstring,
			panda_binaryheaderstring);

	  // We need a catalog object with some elements within it's dictionary
	  openedpdf->catalog = (panda_object *) panda_newobject (openedpdf,
								 panda_normal);
	  panda_adddictitem (openedpdf, openedpdf->catalog, "Type",
			     panda_textvalue, "Catalog");

	  // We need a reference to our pages object
	  panda_addchild (openedpdf->catalog,
			  openedpdf->pages =
			  (panda_object *) panda_newobject (openedpdf,
							    panda_normal));
	  panda_adddictitem (openedpdf, openedpdf->catalog, "Pages",
			     panda_objectvalue, openedpdf->pages);

	  // We need to remember how many pages there are for later
	  openedpdf->pageCount = 0;

	  // And we store a list of the pages we have allocated so far
	  openedpdf->pageholders = panda_xmalloc (sizeof (panda_pagelist));
	  openedpdf->pageholders->next = NULL;

	  // We now need to setup some information in the pages object
	  panda_adddictitem (openedpdf, openedpdf->pages, "Type",
			     panda_textvalue, "Pages");
	  openedpdf->pages->isPages = panda_true;

	  // There is no font currently selected
	  openedpdf->currentFont = NULL;
	  openedpdf->currentFontMode = -1;
	  openedpdf->currentFontSize = -1;
	  openedpdf->nextFontNumber = 1;

	  // The fonts object in the pdf * is a dummy which makes fonts 
	  // external to each page. This makes the PDF more efficient
	  openedpdf->fonts = (panda_object *) panda_newobject (openedpdf,
							       panda_placeholder);

	  // Set the text mode to something basic
	  panda_setfontmode (openedpdf, panda_textmode_normal);
	  panda_setcharacterspacing (openedpdf, 0.0);
	  panda_setwordspacing (openedpdf, 0.0);
	  panda_sethorizontalscaling (openedpdf, 1.0);
	  panda_setleading (openedpdf, 10.0);

	  // Setup the info object with some stuff which makes me happy... :)
	  openedpdf->info = NULL;
	  panda_checkinfo (openedpdf);
	  if (openedpdf->info == NULL)
	    panda_error
	      (panda_true,
	       "Failed to make an info object for the PDF. Not sure why...");

	  // Add some stuff
	  verStr = panda_xsnprintf (
#if defined _WINDOWS
				     "Panda %s MS Windows Version"
#else
				     "Panda %s"
#endif
				     , VERSION);

	  panda_adddictitem (openedpdf, openedpdf->info, "Producer",
			     panda_brackettedtextvalue, verStr);
	  panda_xfree (verStr);
	  panda_adddictitem (openedpdf, openedpdf->info, "CreationDate",
			     panda_brackettedtextvalue, tempPtr =
			     panda_nowdate ());

#if defined DEBUG
	  printf ("Inserted the creation date\n");
#endif

	  if (tempPtr != NULL)
	    {
#if defined DEBUG
	      printf ("Freeing the creation date\n");
#endif
	      panda_xfree (tempPtr);
	    }
	}
      else
	{
	  openedpdf->catalog = NULL;
	  openedpdf->pages = NULL;
	  openedpdf->fonts = NULL;
	}

      // And this stuff is always done

      // Create a dummy object for when we print the pdf to a file
      openedpdf->dummyObj =
	(panda_object *) panda_newobject (openedpdf, panda_placeholder);

      // Remember the mode and create the linear object if needed
      if ((mode[1] == 'l') || (mode[1] == 'L'))
	{
	  openedpdf->mode = panda_writelinear;
	  openedpdf->linear = (panda_object *) panda_newobject (openedpdf,
								panda_normal);
	  panda_adddictitem (openedpdf, openedpdf->linear, "Linearised",
			     panda_integervalue, 1);
	}
      else
	{
	  openedpdf->mode = panda_write;
	  openedpdf->linear = NULL;
	}

#if defined DEBUG
      printf ("PDF file opened ok\n");
#endif

      // We did open the PDF file ok
      return openedpdf;
      break;

    default:
      panda_error (panda_true, "Unknown file I/O mode handed to panda.");
      break;
    }

  // How did we get here?
  return NULL;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_close
PURPOSE write a PDF document out to disk

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_close (panda_pdf *document);
SYNOPSIS END

DESCRIPTION Write out the PDF document we have created to disk, clean up and free memory.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
panda_close(document);
EXAMPLE END
SEEALSO panda_open
DOCBOOK END
******************************************************************************/

// Finish operations on a given PDF document and write the result to disc
void
panda_close (panda_pdf * openedpdf)
{
  panda_xref *xnow, *xprev;
  panda_pagelist *pagelist, *pagevictim;

  // The header was written when we created the file on disk

  // It is now worth our time to count the number of pages and make the count
  // entry in the pages object
  if (openedpdf->pages != NULL)
    panda_adddictitem (openedpdf, openedpdf->pages, "Count",
		       panda_integervalue, openedpdf->pageCount);

  // Before we do anything, we need to make sure that we have ended the
  // textmode if we have entered one on one of the pages. This is because
  // we need to modify the layout stream before we have the possibility of
  // having had it written out to disk
  panda_traverseobjects (openedpdf, openedpdf->pages, panda_down,
			 panda_closetext);

  // We do some different things to write out the PDF depending on the mode
  switch (openedpdf->mode)
    {
    case panda_write:
      // We need to write out the objects into the PDF file and then close the
      // file -- any object which heads an object tree, or lives outside the 
      // tree structure will need a traverseobjects call here...
      if (openedpdf->catalog != NULL)
	panda_traverseobjects (openedpdf, openedpdf->catalog, panda_down,
			       panda_writeobject);

      if (openedpdf->fonts != NULL)
	panda_traverseobjects (openedpdf, openedpdf->fonts, panda_down,
			       panda_writeobject);

      // We need to traverse the dummy object so we pick up the manually 
      // created objects from the lexer
      panda_traverseobjects (openedpdf, openedpdf->dummyObj, panda_down,
			     panda_writeobject);

      if (openedpdf->pages != NULL)
	{
	  // Write our the XREF object -- this MUST happen after all objects 
	  // have been written, or the byte offsets will not be known
	  panda_writexref (openedpdf);

	  // Write the trailer
	  panda_writetrailer (openedpdf);
	}
      break;

    case panda_writelinear:
      // Are there any pages in the PDF? There needs to be at least one...
      if (((panda_child *) openedpdf->pages->children)->me == NULL)
	panda_error (panda_true, "Linearised PDFs need at least one page.");

      panda_writeobject (openedpdf, openedpdf->linear);

      // We need the xref entries for the first page now

      // And the catalog object
      panda_writeobject (openedpdf, openedpdf->catalog);

      // The primary hint stream

      // We now need all of the objects for the first page
      panda_traverseobjects (openedpdf,
			     ((panda_child *) openedpdf->pages->children)->me,
			     panda_down, panda_writeobject);


      break;
    }

  // We also need to free all the memory that we no longer need. This is done
  // separately because sometimes we want to write out but not do this
  // in other words I a=inm leaving space for later movement...
#if defined DEBUG
  printf ("Cleaning up catalog items\n");
#endif

  if (openedpdf->catalog != NULL)
    panda_traverseobjects (openedpdf, openedpdf->catalog, panda_up,
			   panda_freeobject);

#if defined DEBUG
  printf ("Cleaning up font items\n");
#endif

  if (openedpdf->fonts != NULL)
    panda_traverseobjects (openedpdf, openedpdf->fonts, panda_up,
			   panda_freeobject);
  panda_xfree (openedpdf->currentFont);

#if defined DEBUG
  printf ("Cleaning up page holder items\n");
#endif

  pagelist = openedpdf->pageholders;
  while (pagelist->next != NULL)
    {
      pagevictim = pagelist;
      pagelist = pagelist->next;

      panda_xfree (pagevictim->me);

#if defined DEBUG
      printf ("Cleaned up a page item\n");
#endif
    }

  // TODO mikal: this caused a segmentation fault with documents with no
  // pages -- investigate...
  // panda_xfree (pagevictim);

  // Clean up the last one
  panda_xfree (pagelist);

  // Clean up some document level things
  fclose (openedpdf->file);

  while (openedpdf->xrefList->next != NULL)
    {
      xnow = openedpdf->xrefList;
      xprev = NULL;

      while (xnow->next != NULL)
	{
	  xprev = xnow;
	  xnow = xnow->next;
	}

      panda_xfree (xnow);
      if (xprev != NULL)
	xprev->next = NULL;
    }

#if defined DEBUG
  printf ("Total number of objects in document = %d\n",
	  openedpdf->totalObjectNumber);
#endif

  panda_xfree (openedpdf->xrefList);
  panda_dbclose (openedpdf);
  panda_xfree (openedpdf);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_newpage
PURPOSE create a new page in the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
panda_page *panda_newpage(panda_pdf *document, char *pagesize);
SYNOPSIS END

DESCRIPTION Create a new blank page at the end of the PDF with the specified size. Use the standard pagesize strings that are defined by Panda for most things. These are <command>panda_pagesize_a4</command>, and <command>panda_pagesize_usletter</command>. If you need to create your own page sizes, then have a look at these for hints.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
EXAMPLE END
SEEALSO panda_open, panda_close
DOCBOOK END
******************************************************************************/

// Insert a page into the PDF
panda_page *
panda_newpage (panda_pdf * output, char *pageSize)
{
  // Add a page to the PDF
  panda_page *newPage;
  char *pageSizeCopy;

  // The code to setup the page has been moved to make things easier elsewhere
  newPage = panda_createandinsertpage (output);

  // Setup some basic things within the page object's dictionary
  panda_adddictitem (output, newPage->obj, "Type", panda_textvalue, "Page");
  panda_adddictitem (output, newPage->obj, "MediaBox", panda_literaltextvalue,
		     pageSize);
  panda_adddictitem (output, newPage->obj, "Parent", panda_objectvalue,
		     output->pages);
  panda_adddictitem (output, output->pages, "Kids", panda_objectarrayvalue,
		     newPage->obj);

  // Copy the pageSize string somewhere safe, and then clobber the copy.
  // We can't clobber the original because it is a constant anyway and it would
  // be rude to screw with another person's data
  pageSizeCopy =
    (char *) panda_xmalloc (sizeof (char) * (strlen (pageSize) + 1));
  strcpy (pageSizeCopy, pageSize);

  strtok (pageSizeCopy, " ");
  strtok (NULL, " ");
  newPage->width = atoi (strtok (NULL, " "));
  newPage->height = atoi (strtok (NULL, " "));

  panda_xfree (pageSizeCopy);
  return newPage;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_closetext
PURPOSE a traversal callback used for closing text streams within the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_closetext(panda_pdf *document, panda_object *victim);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function is called by the <command>panda_traverseobjects</command>() function to write out streams attached to objects when <command>panda_close</command>() call. You shouldn't need to ever call this function.

RETURNS Nothing

EXAMPLE START
You shouldn't need to use this function.
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_closetext (panda_pdf * opened, panda_object * obj)
{
#if defined DEBUG
  printf ("closetext() traversal struct object numbered %d\n", obj->number);
#endif

  if (obj->textmode == panda_true)
    {
      obj->layoutstream =
	panda_streamprintf (obj->layoutstream, "\nET\n\n\n");
      obj->textmode = panda_false;

#if defined DEBUG
      printf ("Finalised textmode in an object numbered %d\n", obj->number);
#endif
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_processtrans
PURPOSE a traversal callback used for processing transition objects within the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_processtrans(panda_pdf *document, panda_object *victim);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function is called by the <command>panda_traverseobjects</command>() function to deal with transition effects attached to objects when <command>panda_close</command>() call. You shouldn't need to ever call this function.

RETURNS Nothing

EXAMPLE START
You shouldn't need to use this function.
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_processtrans (panda_pdf * opened, panda_object * obj)
{
  // todo_mikal
}
