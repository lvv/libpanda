/******************************************************************************
  Panda.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000

  Purpose:
    Library level calls. These are the calls normally accessible to the
    user, and these calls wrapper our calls.
******************************************************************************/

#define  GLOBALS	here
#include <panda/constants.h>
#include <panda/functions.h>

// Initialise the PDF library ready for operations
void
panda_init ()
{
  int generalCounter;

  // We first need to create the binary string to include in our header
  for (generalCounter = 0; generalCounter <
       (sizeof (gBinaryHeaderString) / sizeof (char)); generalCounter++)
    {
      gBinaryHeaderString[generalCounter] =
	gBinaryChar (gHeaderString[generalCounter]);
    }
}

// Open the named PDF document in the given mode -- suppress means that the
// default panda_objects are not created (used when opening for reading a panda_pdf
// document with the lexer).
panda_pdf *
panda_open (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, gFalse);
}

panda_pdf *
panda_open_suppress (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, gTrue);
}

panda_pdf *
panda_open_actual (char *filename, char *mode, int suppress)
{
  panda_pdf *openedpanda_pdf;
  char *tempPtr;

  // We are going to open a PDF for file I/O. Currently, the only supported
  // mode is 'w'. There are some more obscure modes not included in the panda_error
  // checking below (like a+b), deal with it.

  // Make some space for the PDF information
  openedpanda_pdf = (panda_pdf *) panda_xmalloc(sizeof(panda_pdf));
  
  // Every PDF is going to have to have some panda_xref information associated with
  // it at some stage.
  openedpanda_pdf->panda_xrefList = (panda_xref *) panda_xmalloc(sizeof(panda_xref));
  openedpanda_pdf->panda_xrefList->next = NULL;
  openedpanda_pdf->panda_xrefTail = openedpanda_pdf->panda_xrefList;

  // Do stuff based on the mode of the opening
  switch (mode[0])
    {
    case 'r':
    case 'a':
      panda_error ("Unsupported file I/O mode handed to panda.");
      break;

    case 'w':
      // We are opening a new PDF for writing (we hope)
      if (strlen (mode) > 1)
	switch (mode[1])
	  {
	  case '+':
	    panda_error ("Unsupported file I/O mode handed to panda.");
	    break;
	  }

      // We _are_ going to create the file
      if ((openedpanda_pdf->file = fopen (filename, mode)) == NULL)
	return NULL;

      // We have no panda_objects yet
      openedpanda_pdf->nextObjectNumber = 1;

      // We are at the begining of the file
      openedpanda_pdf->byteOffset = 0;

      // This stuff is avoided by the lexer
      if (suppress == gFalse)
	{
	  // The file will need to have a PDF header to it
	  panda_printf (openedpanda_pdf, "%s%s\n", gMagicHeaderString,
		     gBinaryHeaderString);

	  // We need a catalog panda_object with some elements within it's panda_dictionary
	  openedpanda_pdf->catalog = panda_newpanda_object (openedpanda_pdf, gNormal);
	  panda_adddictitem (openedpanda_pdf->catalog->dict, "Type", gTextValue,
		       "Catalog");

	  // We need a reference to our panda_pages panda_object
	  panda_addpanda_child (openedpanda_pdf->catalog,
		    openedpanda_pdf->panda_pages = panda_newpanda_object (openedpanda_pdf, gNormal));
	  panda_adddictitem (openedpanda_pdf->catalog->dict, "Pages", gObjValue,
		       openedpanda_pdf->panda_pages);

	  // We need to remember how many panda_pages there are for later
	  openedpanda_pdf->panda_pageCount = 0;

	  // We now need to setup some information in the panda_pages panda_object
	  panda_adddictitem (openedpanda_pdf->panda_pages->dict, "Type", gTextValue, "Pages");
	  openedpanda_pdf->panda_pages->isPages = gTrue;

	  // There is no font currently selected
	  openedpanda_pdf->currentFont = NULL;
	  openedpanda_pdf->currentFontSize = -1;
	  openedpanda_pdf->nextFontNumber = 1;

	  // The fonts panda_object in the panda_pdf * is a dummy which makes fonts external
	  // to each panda_page. This makes the PDF more efficient
	  openedpanda_pdf->fonts = panda_newpanda_object (openedpanda_pdf, gPlaceholder);

	  // Set the text mode to something basic
	  panda_setfontmode (openedpanda_pdf, gTextModeNormal);
	  panda_setcharacterspacing (openedpanda_pdf, 0.0);
	  panda_setwordspacing (openedpanda_pdf, 0.0);
	  panda_sethorizontalscaling (openedpanda_pdf, 1.0);
	  panda_setleading (openedpanda_pdf, 0.0);

	  // Setup the info panda_object with some stuff which makes me happy... :)
	  openedpanda_pdf->info = NULL;
	  panda_checkinfo (openedpanda_pdf);
	  if (openedpanda_pdf->info == NULL)
	    panda_error
	      ("Failed to make an info panda_object for the PDF. Not sure why...");

	  // Add some stuff
	  panda_adddictitem (openedpanda_pdf->info->dict, "Producer", gBracketedTextValue,
		       "Panda 0.4");
	  panda_adddictitem (openedpanda_pdf->info->dict, "CreationDate",
		       gBracketedTextValue, tempPtr = panda_nowdate ());
	  free (tempPtr);
	}
      else
	{
	  openedpanda_pdf->catalog = NULL;
	  openedpanda_pdf->panda_pages = NULL;
	  openedpanda_pdf->fonts = NULL;
	}

      // And this stuff is always done

      // Create a dummy panda_object for when we print the panda_pdf to a file
      openedpanda_pdf->dummyObj = panda_newpanda_object (openedpanda_pdf, gPlaceholder);

      // Remember the mode and create the linear panda_object if needed
      if ((mode[1] == 'l') || (mode[1] == 'L'))
	{
	  openedpanda_pdf->mode = gWriteLinear;
	  openedpanda_pdf->linear = panda_newpanda_object (openedpanda_pdf, gNormal);
	  panda_adddictitem (openedpanda_pdf->linear->dict, "Linearised", gIntValue, 1);
	}
      else
	{
	  openedpanda_pdf->mode = gWrite;
	  openedpanda_pdf->linear = NULL;
	}

      // We did open the PDF file ok
      return openedpanda_pdf;
      break;

    default:
      panda_error ("Unknown file I/O mode handed to panda.");
      break;
    }

  // How did we get here?
  return NULL;
}

// Finish operations on a given PDF document and write the result to disc
void
panda_close (panda_pdf * openedpanda_pdf)
{
  panda_xref *xnow, *xprev;

  // The header was written when we created the file on disk

  // It is now worth our time to count the number of panda_pages and make the count
  // entry in the panda_pages panda_object
  if (openedpanda_pdf->panda_pages != NULL)
    panda_adddictitem (openedpanda_pdf->panda_pages->dict, "Count", gIntValue,
		 openedpanda_pdf->panda_pageCount);

  // Before we do anything, we need to make sure that we have ended the
  // textmode if we have entered one on one of the panda_pages. This is because
  // we need to modify the layout stream before we have the possibility of
  // having had it written out to disk
  panda_traversepanda_objects(openedpanda_pdf, openedpanda_pdf->panda_pages, gDown, panda_closetext);

  // We do some different things to write out the PDF depending on the mode
  switch (openedpanda_pdf->mode)
    {
    case gWrite:
      // We need to write out the panda_objects into the PDF file and then close the
      // file -- any panda_object which heads an panda_object tree, or lives outside the 
      // tree structure will need a panda_traversepanda_objects call here...
      if (openedpanda_pdf->catalog != NULL)
	panda_traversepanda_objects (openedpanda_pdf, openedpanda_pdf->catalog, gDown, panda_writepanda_object);

      if (openedpanda_pdf->fonts != NULL)
	panda_traversepanda_objects (openedpanda_pdf, openedpanda_pdf->fonts, gDown, panda_writepanda_object);

      // We need to traverse the dummy panda_object so we pick up the manually 
      // created panda_objects from the lexer
      panda_traversepanda_objects (openedpanda_pdf, openedpanda_pdf->dummyObj, gDown, panda_writepanda_object);

      if (openedpanda_pdf->panda_pages != NULL)
	{
	  // Write our the XREF panda_object -- this MUST happen after all panda_objects 
	  // have been written, or the byte offsets will not be known
	  panda_writepanda_xref (openedpanda_pdf);

	  // Write the trailer
	  panda_writetrailer (openedpanda_pdf);
	}
      break;

    case gWriteLinear:
      // Are there any panda_pages in the PDF? There needs to be at least one...
      if (((panda_child *) openedpanda_pdf->panda_pages->panda_children)->me == NULL)
	panda_error ("Linearised PDFs need at least one panda_page.");

      panda_writepanda_object (openedpanda_pdf, openedpanda_pdf->linear);

      // We need the panda_xref entries for the first panda_page now

      // And the catalog panda_object
      panda_writepanda_object (openedpanda_pdf, openedpanda_pdf->catalog);

      // The primary hint stream

      // We now need all of the panda_objects for the first panda_page
      panda_traversepanda_objects (openedpanda_pdf,
		       ((panda_child *) openedpanda_pdf->panda_pages->panda_children)->me, gDown,
		       panda_writepanda_object);


      break;
    }

  // We also need to free all the memory that we no longer need. This is done
  // separately because sometimes we want to write out but not do this
  // in other words I a=inm leaving space for later movement...
  if (openedpanda_pdf->catalog != NULL)
    panda_traversepanda_objects (openedpanda_pdf, openedpanda_pdf->catalog, gUp, panda_freepanda_object);
  if (openedpanda_pdf->fonts != NULL)
    panda_traversepanda_objects (openedpanda_pdf, openedpanda_pdf->fonts, gUp, panda_freepanda_object);

  // Clean up some document level things
  fclose (openedpanda_pdf->file);

  while (openedpanda_pdf->panda_xrefList->next != NULL)
    {
      xnow = openedpanda_pdf->panda_xrefList;
      xprev = NULL;

      while (xnow->next != NULL)
	{
	  xprev = xnow;
	  xnow = xnow->next;
	}

      free (xnow);
      if (xprev != NULL)
	xprev->next = NULL;
    }

  free (openedpanda_pdf->panda_xrefList);

  free (openedpanda_pdf);
}

// Insert a panda_page into the PDF
panda_page *
panda_newpage (panda_pdf * output, char *panda_pageSize)
{
  // Add a panda_page to the PDF
  panda_page *newPage;
  char *panda_pageSizeCopy;

  // Make some space for the panda_object
  newPage = (panda_page *) panda_xmalloc(sizeof(panda_page));

  // Make the new panda_page panda_object
  newPage->obj = panda_newpanda_object (output, gNormal);

  // Add it to the panda_object tree
  panda_addpanda_child (output->panda_pages, newPage->obj);

  // Setup some basic things within the panda_page panda_object's panda_dictionary
  panda_adddictitem (newPage->obj->dict, "Type", gTextValue, "Page");
  panda_adddictitem (newPage->obj->dict, "MediaBox", gLiteralTextValue, panda_pageSize);
  panda_adddictitem (newPage->obj->dict, "Parent", gObjValue, output->panda_pages);

  // We also need to do the same sort of thing for the contents panda_object
  // that each panda_page owns
  newPage->contents = panda_newpanda_object (output, gNormal);
  panda_addpanda_child (newPage->obj, newPage->contents);
  panda_adddictitem (newPage->obj->dict, "Contents", gObjValue, newPage->contents);

  // Copy the panda_pageSize string somewhere safe, and then clobber the copy.
  // We can't clober the original because it is a constant anyway and it would
  // be rude to screw with another person's data
  panda_pageSizeCopy = (char *) panda_xmalloc(sizeof (char) * (strlen (panda_pageSize) + 1));
  strcpy (panda_pageSizeCopy, panda_pageSize);

  strtok (panda_pageSizeCopy, " ");
  strtok (NULL, " ");
  newPage->width = atoi (strtok (NULL, " "));
  newPage->height = atoi (strtok (NULL, " "));

  free (panda_pageSizeCopy);

  // Increment the panda_page count for the PDF
  output->panda_pageCount++;

  return newPage;
}

void
panda_closetext(panda_pdf *opened, panda_object *obj){
#if defined DEBUG
  printf("panda_closetext() traversal struct panda_object numbered %d\n", obj->number);
#endif

  if(obj->textmode == gTrue)
    {
      obj->layoutstream =
	panda_streamprintf(obj->layoutstream, "\nET\n\n\n");
      obj->textmode = gFalse;

#if defined DEBUG
      printf("Finalised textmode in an panda_object numbered %d\n",
	     obj->number);
#endif
    }
}
  
