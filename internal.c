/******************************************************************************
  internal.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      18012001

  Purpose:
    These are calls that are internal, but more generally useful than those
    found in some of the functionality specific code files...
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

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
  else if(target->contents->insidegraphicsblock == panda_false)
    {
      target->contents->layoutstream =
	panda_streamprintf (target->contents->layoutstream, "q\n");
      target->contents->insidegraphicsblock = panda_true;
    }
}

void
panda_exitgraphicsmode (panda_page * target)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "Q\n\n");
  target->contents->insidegraphicsblock = panda_false;
}

panda_page *
panda_createandinsertpage (panda_pdf * output){
  panda_page *newPage;

  // Make some space for the object
  newPage = (panda_page *) panda_xmalloc (sizeof (panda_page));

  // Make the new page object
  newPage->obj = panda_newobject (output, panda_normal);

  // Add it to the object tree
  panda_addchild (output->pages, newPage->obj);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  newPage->contents = panda_newobject (output, panda_normal);
  panda_addchild (newPage->obj, newPage->contents);
  panda_adddictitem (newPage->obj->dict, "Contents", panda_objectvalue,
		     newPage->contents);

  // Setup some stuff in the contents object that we need later
  newPage->contents->textmode = panda_false;
  newPage->contents->insidegraphicsblock = panda_false;

  // Increment the page count for the PDF
  output->pageCount++;

  return newPage;
}
