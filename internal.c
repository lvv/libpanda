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
  else
    {
      target->contents->layoutstream =
	panda_streamprintf (target->contents->layoutstream, "q\n");
    }
}

void
panda_exitgraphicsmode (panda_page * target)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "Q\n\n");
}
