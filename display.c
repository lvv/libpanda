/******************************************************************************
  display.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      31032001

  Purpose:
    This file looks after things that the user can do to change the way the
    PDF document displays within viewers. Most of these things might not be
    supported by all viewers.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinestart
PURPOSE sets the starting point of a curve

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinestart (panda_page * target, int x, int y);
SYNOPSIS END

DESCRIPTION Set the starting point for the sequence of curves and lines that it to be drawn on the current page. This call is compulsory for almost all of the line drawing functions. It is not required for the <command>panda_rectangle</command> call.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_setlinestart (page, 100, 200);
EXAMPLE END
SEEALSO 
DOCBOOK END
******************************************************************************/

// Set the start point of a line on the page
void
panda_setlinestart (panda_page * target, int x, int y)
{
  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d m\n", x, target->height - y);
}
