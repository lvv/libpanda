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
panda_setlinestart(panda_page *target, int x, int y)
{
  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "%d %d m\n", x, target->height - y);
}

// Add a point to the line that we are drawing (a straight line segment)
void
panda_addlinesegment(panda_page *target, int x, int y)
{
  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "%d %d l\n", x, target->height - y);
}

// Add a curved segment to the line that we are building
void
panda_addcubiccurvesegment(panda_page *target, int x, int y, int cx1, int cy1,
			   int cx2, int cy2)
{
  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "%d %d %d %d %d %d v\n", cx1, cy1, cx2, cy2, x, y);
}

// Add a different type of curved segment
void
panda_addquadraticcurvedsegment(panda_page *target, int x, int y, int cx1,
				int cy1)
{
  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "%d %d %d %d y\n", cx1, cy1, x, y);
}

// Return to the start point of the line to make some
void
panda_closeline(panda_page *)
{
  target->contents->layoutstream =
    panda_streamprintf(target->contents->layoutstream, "h\n");
}

// Make the line be drawn by the viewer
void
panda_endline(panda_page *target)
{
  target->contents->layoutstream = 
    panda_streamprintf(target->contents->layoutstream,
		       "S\n");
}


