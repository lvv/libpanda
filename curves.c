/******************************************************************************
  curves.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      18012001

  Purpose:
    This file looks after the calls to draw lines and curves onto a PDF page.
    It also deals with things like the line ending styles and good stuff like
    that. Many of these calls are internal, and are abstracted for real use
    later in this file...
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

// Set the start point of a line on the page
void
panda_setlinestart(panda_page *target)
{
  //  panda_entergraphicsmode(target);

  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "100 100 m\n200 200 l\nS\n");

  //  panda_exitgraphicsmode(target);
}




