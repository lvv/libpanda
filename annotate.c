/******************************************************************************
  Annotate.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      07012002

  Purpose:
    Annotation stuff

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

FUNCTION panda_textannotation
PURPOSE draw a text annotation which looks like a sticky note onto a page

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_textannotation(panda_pdf *document, panda_page *page, int open,
		     char *text, int top, int left, int bottom, int right,
		     int icon, int flags);
SYNOPSIS END

DESCRIPTION This function inserts a text annotation onto one of the pages in your PDF document. The open arguement defines whether the text annotation initially appears with the text visible (the open state) or not. The possible values for the icon arguement are: panda_icon_comment; panda_icon_help; panda_icon_insert; panda_icon_key; panda_icon_newparagraph; panda_icon_note; panda_icon_paragraph. The possible values for the flags arguement are: panda_annotationflag_invisible; panda_annotationflag_hidden; panda_annotationflag_print; panda_annotationflag_nozoom; panda_annotationflag_norotate; panda_annotationflag_noview; panda_annotationflag_readonly -- the values for this arguement may by OR'ed together if needed.
</para>
<para>
<command>Please note that not all PDF viewers support annotations, so you cannot gaurantee that the user will see this annotation...</command> 

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *currPage;

panda_init();
if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");
currPage = panda_newpage (demo, panda_pagesize_a4);

panda_textannotation(demo, currPage, panda_true, "Hello", 10, 10, 50, 50,
                       panda_icon_comment, 0);
EXAMPLE END
SEEALSO panda_freetextannotation, panda_insertannotation
DOCBOOK END
******************************************************************************/

// TODO: docbook out of date
void
panda_textannotation (panda_pdf * document, panda_page * page, int open,
		      char *text, char *title,
		      int top, int left, int bottom, int right,
		      double red, double green, double blue,
		      int icon, int flags)
{
  panda_object *annotation;

#if defined DEBUG
  printf ("Inserting a text annotation\n");
#endif

  annotation = panda_insertannotation (document, page, title,
				       top, left, bottom, right,
				       red, green, blue, flags);

  panda_adddictitem (document, annotation, "Open", panda_booleanvalue, open);
  panda_adddictitem (document, annotation, "Subtype",
		     panda_textvalue, "Text");
  panda_adddictitem (document, annotation, "Contents",
		     panda_brackettedtextvalue, text);

  // Icons
  switch (icon)
    {
    case panda_icon_comment:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Comment");
      break;

    case panda_icon_help:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Help");
      break;

    case panda_icon_insert:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Insert");
      break;

    case panda_icon_key:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Key");
      break;

    case panda_icon_newparagraph:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "NewParagraph");
      break;

    case panda_icon_note:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Note");
      break;

    case panda_icon_paragraph:
      panda_adddictitem (document, annotation, "Name",
			 panda_textvalue, "Paragraph");
      break;
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_freetextannotation
PURPOSE draw a text annotation which doesn't look like a sticky note onto a page

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_freetextannotation(panda_pdf *document, panda_page *page,
		     char *text, int top, int left, int bottom, int right,
		     int flags);
SYNOPSIS END

DESCRIPTION This function inserts a text annotation onto one of the pages in your PDF document. The possible values for the flags arguement are: panda_annotationflag_invisible; panda_annotationflag_hidden; panda_annotationflag_print; panda_annotationflag_nozoom; panda_annotationflag_norotate; panda_annotationflag_noview; panda_annotationflag_readonly -- the values for this arguement may by OR'ed together if needed.
</para>
<para>
<command>Please note that not all PDF viewers support annotations, so you cannot gaurantee that the user will see this annotation...</command> 

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *currPage;

panda_init();
if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");
currPage = panda_newpage (demo, panda_pagesize_a4);

panda_freetextannotation(demo, currPage, "Hello", 10, 10, 50, 50, 0);
EXAMPLE END
SEEALSO panda_textannotation, panda_insertannotation
DOCBOOK END
******************************************************************************/

// TODO: docbook out of date
void
panda_freetextannotation (panda_pdf * document, panda_page * page,
			  char *text, char *title,
			  int top, int left, int bottom, int right,
			  double red, double green, double blue, int flags)
{
  panda_object *annotation;

#if defined DEBUG
  printf ("Inserting a freetext annotation\n");
#endif

  annotation = panda_insertannotation (document, page, title,
				       top, left, bottom, right,
				       red, green, blue, flags);

  panda_adddictitem (document, annotation, "Subtype",
		     panda_textvalue, "FreeText");
  panda_adddictitem (document, annotation, "Contents",
		     panda_brackettedtextvalue, text);
  //panda_adddictitem(document, annotation, "DA",
  //                panda_textvalue, 

  // todo_mikal
}

void
panda_lineannotation (panda_pdf * document, panda_page * page,
		      char *text, char *title, int x1, int y1, int x2, int y2,
		      int top, int left, int bottom, int right,
		      double red, double green, double blue, int flags)
{
  panda_object *annotation;
  char *lineString;

#if defined DEBUG
  printf ("Inserting a line annotation\n");
#endif

  annotation =
    panda_insertannotation (document, page, title, top, left, bottom, right,
			    red, green, blue, flags);

  panda_adddictitem (document, annotation, "Subtype",
		     panda_textvalue, "Line");
  panda_adddictitem (document, annotation, "Contents",
		     panda_brackettedtextvalue, text);

  lineString = panda_xsnprintf ("[%d %d %d %d]", x1, page->height - y1,
				x2, page->height - y2);
  panda_adddictitem (document, annotation, "L",
		     panda_literaltextvalue, lineString);
  panda_xfree (lineString);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_insertannotation
PURPOSE actually insert an annotation

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_object *panda_insertannotation(panda_pdf *document, panda_page *page,
				     int top, int left, int bottom, int right,
				     int flags)
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call creates the object to store the annotation and the common elements which are required.

RETURNS The annotation object which was created.

EXAMPLE START
You shouldn't need an example because this is an internal call
EXAMPLE END
SEEALSO panda_textannotation, panda_freetextannotation
DOCBOOK END
******************************************************************************/

// TODO: docbook out of date
panda_object *
panda_insertannotation (panda_pdf * document, panda_page * page,
			char *title,
			int top, int left, int bottom, int right,
			double red, double green, double blue, int flags)
{
  panda_object *annotation;
  char *rectString, *dateString, *colourString;

  // Each annotation is stored in it's own object because we can
  annotation = panda_newobject (document, panda_normal);
  panda_addchild (document->pages, annotation);

  // Add this to the list of annotations for the named page
  panda_adddictitem (document, page->obj, "Annots",
		     panda_objectarrayvalue, annotation);
  panda_adddictitem (document, annotation, "Type", panda_textvalue, "Annot");
  panda_adddictitem (document, annotation, "F", panda_integervalue, flags);
  panda_adddictitem (document, annotation, "M",
		     panda_brackettedtextvalue, dateString =
		     panda_nowdate ());
  panda_xfree (dateString);
  panda_adddictitem (document, annotation, "P", panda_objectvalue, page->obj);
  panda_adddictitem (document, annotation, "T",
		     panda_brackettedtextvalue, title);

  // The colour of the annotation
  colourString = panda_xsnprintf ("[%.2f %.2f %.2f]", red, green, blue);
  panda_adddictitem (document, annotation, "C",
		     panda_literaltextvalue, colourString);
  panda_xfree (colourString);

  // The onscreen rectangle
  rectString = panda_xsnprintf ("[%d %d %d %d]", left, page->height - bottom,
				right, page->height - top);
  panda_adddictitem (document, annotation, "Rect",
		     panda_literaltextvalue, rectString);
  panda_xfree (rectString);

  return annotation;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_internallink
PURPOSE do work common to panda_urilink and panda_link

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

void panda_internallink (panda_pdf * document, panda_object * link,
		    panda_page * page, int top, int left, int bottom,
		    int right, int border)
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function will create set up common properties of a link annotation.

RETURNS Nothing.

EXAMPLE START
You shouldn't need an example because this is an internal call
EXAMPLE END
SEEALSO panda_urilink, panda_link
DOCBOOK END
******************************************************************************/

void
panda_internallink (panda_pdf * document, panda_object * link,
		    panda_page * page, int top, int left, int bottom,
		    int right, int border)
{
  char *tmpPtr;

  panda_adddictitem (document, page->obj, "Annots", panda_objectarrayvalue,
		     link);
  panda_adddictitem (document, link, "Type", panda_textvalue, "Annot");
  panda_adddictitem (document, link, "Subtype", panda_textvalue, "Link");
  panda_adddictitem (document, link, "Rect", panda_literaltextvalue,
		     tmpPtr =
		     panda_xsnprintf ("[%d %d %d %d]", left,
				      page->height - bottom, right,
				      page->height - top));
  panda_xfree (tmpPtr);
  panda_adddictitem (document, link, "Border", panda_literaltextvalue,
		     tmpPtr = panda_xsnprintf ("[0 0 %d]", border));
  panda_xfree (tmpPtr);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_urilink
PURPOSE insert link to a URI.

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

void panda_urilink (panda_pdf * document, panda_page * page, char *uri, int top,
	       int left, int bottom, int right, int border)
SYNOPSIS END

DESCRIPTION This function will create a link to a URI based on the supplied arguments

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *currPage;

panda_init();
if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");
currPage = panda_newpage (demo, panda_pagesize_a4);

panda_urilink (demo, currPage, "http://www.google.com/", 5, 5, 100, 100, 1);
EXAMPLE END
SEEALSO panda_link
DOCBOOK END
******************************************************************************/

void
panda_urilink (panda_pdf * document, panda_page * page, char *uri, int top,
	       int left, int bottom, int right, int border)
{
  panda_object *link;
  char *tmpPtr;


  link = panda_newobject (document, panda_normal);
  panda_addchild (document->pages, link);

  panda_internallink (document, link, page, top, left, bottom, right, border);
  panda_adddictitem (document, link, "A", panda_literaltextvalue,
		     tmpPtr =
		     panda_xsnprintf ("<< /S /URI /URI (%s) >>", uri));
  panda_xfree (tmpPtr);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_link
PURPOSE Create a link to an internal location in a PDF.

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

void
panda_link (panda_pdf * document, panda_page * dest, int dest_top,
	    int dest_left, int zoom, panda_page * source, int top, int left,
	    int bottom, int right, int border)
SYNOPSIS END

DESCRIPTION This function will create a link to the specified location, at a specified location.

RETURNS Nothing.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *demo;
panda_page *Page1, *Page2;

panda_init();
if ((demo = panda_open ("output.pdf", "w")) == NULL)
    panda_error (panda_true, "demo: could not open output.pdf to write to.");
Page1 = panda_newpage (demo, panda_pagesize_a4);
Page2 = panda_newpage (demo, panda_pagesize_a4);

panda_link (demo, Page1, 0, 0, 0, Page2, 5, 5, 100, 100, 1);
EXAMPLE END
SEEALSO panda_urilink
DOCBOOK END
******************************************************************************/

void
panda_link (panda_pdf * document, panda_page * dest, int dest_top,
	    int dest_left, int zoom, panda_page * source, int top, int left,
	    int bottom, int right, int border)
{
  panda_object *link;
  char *tmpPtr;


  link = panda_newobject (document, panda_normal);
  panda_addchild (document->pages, link);

  panda_internallink (document, link, source, top, left, bottom, right,
		      border);
  panda_adddictitem (document, link, "Dest", panda_literaltextvalue, tmpPtr =
		     panda_xsnprintf ("[%d %d R /XYZ %d %d %d]",
				      dest->obj->number,
				      dest->obj->generation, dest_left,
				      dest->height - dest_top, zoom));
  panda_xfree (tmpPtr);
}
