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
       (sizeof (panda_binaryheaderstring) / sizeof (char)); generalCounter++)
    {
      panda_binaryheaderstring[generalCounter] =
	panda_binarychar (panda_headerstring[generalCounter]);
    }
}

// Open the named PDF document in the given mode -- suppress means that the
// default objects are not created (used when opening for reading a pdf
// document with the lexer).
panda_pdf *
panda_open (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, panda_false);
}

panda_pdf *
panda_open_suppress (char *filename, char *mode)
{
  return panda_open_actual (filename, mode, panda_true);
}

panda_pdf *
panda_open_actual (char *filename, char *mode, int suppress)
{
  panda_pdf *openedpdf;
  char *tempPtr;

  // We are going to open a PDF for file I/O. Currently, the only supported
  // mode is 'w'. There are some more obscure modes not included in the error
  // checking below (like a+b), deal with it.

  // Make some space for the PDF information
  openedpdf = (panda_pdf *) panda_xmalloc (sizeof (panda_pdf));

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
      if ((openedpdf->file = fopen (filename, mode)) == NULL)
	return NULL;

      // We have no objects yet
      openedpdf->nextObjectNumber = 1;

      // We are at the begining of the file
      openedpdf->byteOffset = 0;

      // This stuff is avoided by the lexer
      if (suppress == panda_false)
	{
	  // The file will need to have a PDF header to it
	  panda_printf (openedpdf, "%s%s\n", panda_magicheaderstring,
			panda_binaryheaderstring);

	  // We need a catalog object with some elements within it's dictionary
	  openedpdf->catalog = panda_newobject (openedpdf, panda_normal);
	  panda_adddictitem (openedpdf->catalog->dict, "Type",
			     panda_textvalue, "Catalog");

	  // We need a reference to our pages object
	  panda_addchild (openedpdf->catalog,
			  openedpdf->pages =
			  panda_newobject (openedpdf, panda_normal));
	  panda_adddictitem (openedpdf->catalog->dict, "Pages",
			     panda_objectvalue, openedpdf->pages);

	  // We need to remember how many pages there are for later
	  openedpdf->pageCount = 0;

	  // We now need to setup some information in the pages object
	  panda_adddictitem (openedpdf->pages->dict, "Type", panda_textvalue,
			     "Pages");
	  openedpdf->pages->isPages = panda_true;

	  // There is no font currently selected
	  openedpdf->currentFont = NULL;
	  openedpdf->currentFontSize = -1;
	  openedpdf->nextFontNumber = 1;

	  // The fonts object in the pdf * is a dummy which makes fonts external
	  // to each page. This makes the PDF more efficient
	  openedpdf->fonts = panda_newobject (openedpdf, panda_placeholder);

	  // Set the text mode to something basic
	  panda_setfontmode (openedpdf, panda_textmode_normal);
	  panda_setcharacterspacing (openedpdf, 0.0);
	  panda_setwordspacing (openedpdf, 0.0);
	  panda_sethorizontalscaling (openedpdf, 1.0);
	  panda_setleading (openedpdf, 0.0);

	  // Setup the info object with some stuff which makes me happy... :)
	  openedpdf->info = NULL;
	  panda_checkinfo (openedpdf);
	  if (openedpdf->info == NULL)
	    panda_error
	      ("Failed to make an info object for the PDF. Not sure why...");

	  // Add some stuff
	  panda_adddictitem (openedpdf->info->dict, "Producer",
			     panda_brackettedtextvalue, "Panda 0.4");
	  panda_adddictitem (openedpdf->info->dict, "CreationDate",
			     panda_brackettedtextvalue, tempPtr =
			     panda_nowdate ());
	  free (tempPtr);
	}
      else
	{
	  openedpdf->catalog = NULL;
	  openedpdf->pages = NULL;
	  openedpdf->fonts = NULL;
	}

      // And this stuff is always done

      // Create a dummy object for when we print the pdf to a file
      openedpdf->dummyObj = panda_newobject (openedpdf, panda_placeholder);

      // Remember the mode and create the linear object if needed
      if ((mode[1] == 'l') || (mode[1] == 'L'))
	{
	  openedpdf->mode = panda_writelinear;
	  openedpdf->linear = panda_newobject (openedpdf, panda_normal);
	  panda_adddictitem (openedpdf->linear->dict, "Linearised",
			     panda_integervalue, 1);
	}
      else
	{
	  openedpdf->mode = panda_write;
	  openedpdf->linear = NULL;
	}

      // We did open the PDF file ok
      return openedpdf;
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
panda_close (panda_pdf * openedpdf)
{
  panda_xref *xnow, *xprev;

  // The header was written when we created the file on disk

  // It is now worth our time to count the number of pages and make the count
  // entry in the pages object
  if (openedpdf->pages != NULL)
    panda_adddictitem (openedpdf->pages->dict, "Count", panda_integervalue,
		       openedpdf->pageCount);

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
	panda_error ("Linearised PDFs need at least one page.");

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
  if (openedpdf->catalog != NULL)
    panda_traverseobjects (openedpdf, openedpdf->catalog, panda_up,
			   panda_freeobject);
  if (openedpdf->fonts != NULL)
    panda_traverseobjects (openedpdf, openedpdf->fonts, panda_up,
			   panda_freeobject);

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

      free (xnow);
      if (xprev != NULL)
	xprev->next = NULL;
    }

  free (openedpdf->xrefList);

  free (openedpdf);
}

// Insert a page into the PDF
panda_page *
panda_newpage (panda_pdf * output, char *pageSize)
{
  // Add a page to the PDF
  panda_page *newPage;
  char *pageSizeCopy;

  ///////////////////////////////////////////////////////////////////////////
  // A template page is treated as a special case of a page, because they are
  // similar in many respects
  ///////////////////////////////////////////////////////////////////////////
  //  if(strcmp(pageSize, gPageSizeTemplate) == 0){ 
  //    pageSizeCopy = (char *) panda_xmalloc(sizeof(char) * 
  //                                      (strlen(pageSize) + 1));
  //    strcpy(pageSizeCopy, pageSzie);
  //    strtok(pageSizeCopy, " ");
  //    newPage = panda_newtemplate(output, strtok(NULL, " "));
  //    free(pageSizeCopy);
  //   return newPage;
  // }

  // Make some space for the object
  newPage = (panda_page *) panda_xmalloc (sizeof (panda_page));

  // Make the new page object
  newPage->obj = panda_newobject (output, panda_normal);

  // Add it to the object tree
  panda_addchild (output->pages, newPage->obj);

  // Setup some basic things within the page object's dictionary
  panda_adddictitem (newPage->obj->dict, "Type", panda_textvalue, "Page");
  panda_adddictitem (newPage->obj->dict, "MediaBox", panda_literaltextvalue,
		     pageSize);
  panda_adddictitem (newPage->obj->dict, "Parent", panda_objectvalue,
		     output->pages);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  newPage->contents = panda_newobject (output, panda_normal);
  panda_addchild (newPage->obj, newPage->contents);
  panda_adddictitem (newPage->obj->dict, "Contents", panda_objectvalue,
		     newPage->contents);

  // Copy the pageSize string somewhere safe, and then clobber the copy.
  // We can't clober the original because it is a constant anyway and it would
  // be rude to screw with another person's data
  pageSizeCopy =
    (char *) panda_xmalloc (sizeof (char) * (strlen (pageSize) + 1));
  strcpy (pageSizeCopy, pageSize);

  strtok (pageSizeCopy, " ");
  strtok (NULL, " ");
  newPage->width = atoi (strtok (NULL, " "));
  newPage->height = atoi (strtok (NULL, " "));

  free (pageSizeCopy);

  // Increment the page count for the PDF
  output->pageCount++;

  return newPage;
}

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
