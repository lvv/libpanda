/******************************************************************************
  curves.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      18012001

  Purpose:
    This file looks after the calls to draw lines and curves onto a PDF page.
    It also deals with things like the line ending styles and good stuff like
    that. Many of these calls are internal, and are abstracted for real use
    later in this file...

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
SEEALSO panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
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

/******************************************************************************
DOCBOOK START

FUNCTION panda_addlinesegment
PURPOSE add a straight segment to the line shape we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_addlinesegment (panda_page * target, int x, int y);
SYNOPSIS END

DESCRIPTION Add a point to the shape we are currently drawing with a straight line between the current cursor location and (x,y).

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
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_addlinesegment (panda_page * target, int x, int y)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d l\n", x, target->height - y);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_addcubiccurvesegment
PURPOSE add a curved segment to the line shape we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_addcubiccurvesegment (panda_page * target, int x, int y, int cx1, int cy1, int cx2, int cy2);
SYNOPSIS END

DESCRIPTION Add a point to the shape we are currently drawing with a cubic curve between the current cursor location and (x,y). There are two control points used to generate the cubic curve. They are (cx1, cy1) and (cx2, cy2).

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_setlinestart (currPage, 210, 210);
panda_addcubiccurvesegment (currPage, 310, 210, 225, 300, 275, 400);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

// Add a curved segment to the line that we are building
void
panda_addcubiccurvesegment (panda_page * target, int x, int y, int cx1,
			    int cy1, int cx2, int cy2)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d %d %d c\n", cx1, target->height - cy1,
			cx2, target->height - cy2, x, target->height - y);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_addquadraticcurvesegmentone
PURPOSE add a curved segment to the line shape that we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_addquadraticcurvesegmentone (panda_page * target, int x, int y, int cx1, int cy1);
SYNOPSIS END

DESCRIPTION This function adds a curved segment to the line shape that we are drawing. The curved segment has a control point, namely (cx1, cy1). This call creates slightly different curves from <command>panda_addquadraticcurvesegmenttwo</command>();

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
panda_addquadraticcurvesegmentone (page, 200, 200, 12, 32);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

// Add a different type of curved segment
void
panda_addquadraticcurvesegmentone (panda_page * target, int x, int y, int cx1,
				   int cy1)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d v\n", cx1, target->height - cy1,
			x, target->height - y);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_addquadraticcurvesegmenttwo
PURPOSE add a curved segment to the line shape that we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_addquadraticcurvesegmenttwo (panda_page * target, int x, int y, int cx1, int cy1);
SYNOPSIS END

DESCRIPTION This function adds a curved segment to the line shape that we are drawing. The curved segment has a control point, namely (cx1, cy1). This call creates slightly different curves from <command>panda_addquadraticcurvesegmentone</command>();

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
panda_addquadraticcurvesegmenttwo (page, 200, 200, 12, 32);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_addquadraticcurvesegmenttwo (panda_page * target, int x, int y, int cx1,
				   int cy1)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d y\n", cx1, target->height - cy1,
			x, target->height - y);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_closeline
PURPOSE close off the line shape we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_closeline(panda_page * target);
SYNOPSIS END

DESCRIPTION Close the line shape we are drawing by returning to the starting point as set by <command>panda_setlinestart</command>();

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
panda_addlinesegment (page, 200, 200);
panda_addlinesegment (page, 400, 300);
panda_closeline (page);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_closeline (panda_page * target)
{
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "h\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_rectangle
PURPOSE draw a rectangle

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_rectangle (panda_page * target, int top, int left, int bottom, int right);
SYNOPSIS END

DESCRIPTION Draw a rectangle on the PDF page. There is no need for the <command>panda_setlinestart</command>() or <command>panda_closeline</command>() calls.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);

panda_rectangle( page, 10, 10, 150, 200);
EXAMPLE END
SEEALSO panda_setlinestart,  panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_rectangle (panda_page * target, int top, int left, int bottom,
		 int right)
{
  panda_entergraphicsmode (target);

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%d %d %d %d re\n", left, target->height - top,
			right - left, top - bottom);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_endline
PURPOSE finalise the current line shape

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_endline( panda_page *target);
SYNOPSIS END

DESCRIPTION Finalise the line shape we are drawing. Only one line shape may be drawn at any one time. There is no need for this call with the <command>panda_rectangle</command>() call.

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
panda_addlinesegment (page, 200, 200);
panda_endline (page);
EXAMPLE END
SEEALSO panda_setlinestart,  panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_endline (panda_page * target)
{
  panda_exitgraphicsmode (target);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_strokeline
PURPOSE stroke the line shape we have just drawn

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_strokeline (panda_page * target);
SYNOPSIS END

DESCRIPTION This function must be called for the line shape that we have drawn to actually display on the PDF page. This process is known as 'stroking', and hence the name of this function call.

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
panda_addlinesegment (page, 200, 200);
panda_strokeline (page);
panda_endline (page);
EXAMPLE END
SEEALSO panda_setlinestart,  panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_strokeline (panda_page * target)
{
  // There is also an option to close and stoke, but I see no reason to
  // implement it...
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "S\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_fillline
PURPOSE fill the closed shape we just drew

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_fillline (panda_page * target);
SYNOPSIS END

DESCRIPTION Fill the shape we have just drawn with the previously defined fill.

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
panda_addlinesegment (page, 200, 200);
panda_addlinesegment (page, 250, 250);
panda_endline (page);
panda_fillline (page);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_fillline (panda_page * target)
{
  // There is another instance of this command, but the spec recommends not
  // implementing it...
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "B\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinewidth
PURPOSE sets the width of the line that we are drawing

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinewidth (panda_page * target, int width);
SYNOPSIS END

DESCRIPTION Set the width of the line that is being drawn... You can use <command>panda_setlinecap</command>(), <command>panda_setlinejoin</command>() and <command>panda_setlinedash</command>() to change other characteristics of the line.

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
panda_setlinewidth (page, 42);
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_setlinewidth (panda_page * target, int width)
{
  // The width must be positive
  if (width < 0)
    {
      fprintf (stderr, "Panda ignoring bad width\n");
      return;
    }

  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "%d w\n", width);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinecap
PURPOSE sets the line cap for the lines we are drawing now

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinecap ( panda_page *target, int cap);
SYNOPSIS END

DESCRIPTION A line cap is used at the ends of lines that do not meet other lines. The different cap styles are defined in panda/constants.h and are: panda_linecap_butt, panda_linecap_round and panda_linecap_projectedsquare.

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
panda_setlinecap (page, panda_linecap_round);
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinejoin, panda_setlinedash, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_setlinecap (panda_page * target, int cap)
{
  // The width must be positive
  if ((cap < 0) || (cap >= panda_linecap_max))
    {
      fprintf (stderr, "Panda ignoring bad line cap\n");
      return;
    }

  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "%d J\n", cap);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinejoin
PURPOSE is used to set the line join style

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinejoin (panda_page *target, int join);
SYNOPSIS END

DESCRIPTION A line join is used where the ends of two lines meet. The valid line joins are defined in panda/constants.h and are: panda_linejoin_miter, panda_linejoin_round and panda_linejoin_bevel.

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
panda_setlinejoin (page, panda_linejoin_bevel);
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_setlinejoin (panda_page * target, int join)
{
  // The width must be positive
  if ((join < 0) || (join >= panda_linejoin_max))
    {
      fprintf (stderr, "Panda ignoring bad line join\n");
      return;
    }

  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "%d j\n", join);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinedashing
PURPOSE draw the subsequent lines with the defined dashing pattern

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinedashing (panda_page *target, int on, int off, int pahse);
SYNOPSIS END

DESCRIPTION This function allows the user to define a line dashing style, which is then applied to subsequent lines drawn on that page. The dashing style is defined as a on and off number, as well as a phase. For example, on = 2, off = 4, phase = 0 should result in a line like:
</para>

<para>
<programlisting>
--    --    --    --    --    --    --
</programlisting>
</para>

<para>
Whilst values of on = 2, off = 4, phase = 1 would give:
</para>

<para>
<programlisting>
    --    --    --    --    --    --    --
</programlisting>
</para>

<para>
You get the idea...

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
panda_setlinedash (page, 2, 4, 0);
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setfillcolor, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_setlinedash (panda_page * target, int on, int off, int phase)
{
  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"[%d %d] %d d\n", on, off, phase);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setfillcolor
PURPOSE set the color to fill a close shape with

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setfillcolor (panda_page *target, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION This function sets the color to fill a close shape with when the shape is closed. It is expressed as a combinartion of red, green, and blue. The maximum number for each value is 255 (a number greater than 255 is reduced to 255).

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
panda_setfillcolor (page, 100, 200, 300);
panda_addlinesegment (page, 200, 200);
panda_addlinesegment (page, 250, 300);
panda_closeline (page);
panda_endline (page);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setlinecolor
DOCBOOK END
******************************************************************************/

void
panda_setfillcolor (panda_page * target, int red, int green, int blue)
{
  if (red > 255)
    red = 255;
  if (green > 255)
    green = 255;
  if (blue > 255)
    blue = 255;

  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%f %f %f rg\n", (((double) red) / 255),
			(((double) green) / 255), (((double) blue) / 255));
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setlinecolor
PURPOSE change the color of the line drawn

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setlinecolor (panda_page *target, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the color of lines being drawn using a combination of red, green and blue.

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
panda_setlinecolor (page, 100, 200, 450);
panda_addlinesegment (page, 200, 200);
EXAMPLE END
SEEALSO panda_setlinestart, panda_addlinesegment, panda_addcubiccurvesegment, panda_addquadraticsegmentone, panda_addquadraticcurvesegmenttwo, panda_closeline, panda_rectangle, panda_endline, panda_strokeline, panda_fillline, panda_setlinewidth, panda_setlinecap, panda_setlinejoin, panda_setlinedash, panda_setfillcolor
DOCBOOK END
******************************************************************************/

void
panda_setlinecolor (panda_page * target, int red, int green, int blue)
{
  if (red > 255)
    red = 255;
  if (green > 255)
    green = 255;
  if (blue > 255)
    blue = 255;

  panda_entergraphicsmode (target);
  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream,
			"%f %f %f RG\n", (((double) red) / 255),
			(((double) green) / 255), (((double) blue) / 255));
}
