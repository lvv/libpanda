/******************************************************************************
  Annotate.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      07012002

  Purpose:
    Annotation stuff
******************************************************************************/

#if defined _WINDOWS
#include "panda/constants.h"
#include "panda/functions.h"
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

void
panda_textannotation(panda_pdf *document, panda_page *page, int open,
		     char *text, int top, int left, int bottom, int right,
		     int icon, int flags)
{
  panda_object *annotation;

  annotation = panda_insertannotation(document, page, top, left, bottom, right,
				      flags);
  
  panda_adddictitem(document, annotation, "Open",
		    panda_booleanvalue, open);
  panda_adddictitem(document, annotation, "Subtype", 
		    panda_textvalue, "Text");
  panda_adddictitem(document, annotation, "Contents", 
		    panda_brackettedtextvalue, text);  

  // Icons
  switch(icon){
  case panda_icon_comment:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "Comment");
    break;

  case panda_icon_help:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "Help");
    break;

  case panda_icon_insert:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "Insert");
    break;

  case panda_icon_key:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "Key");
    break;

  case panda_icon_newparagraph:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "NewParagraph");
    break;

  case panda_icon_note:
    panda_adddictitem(document, annotation, "Name",
		      panda_textvalue, "Note");
    break;

  case panda_icon_paragraph:
    panda_adddictitem(document, annotation, "Name",
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

void
panda_freetextannotation(panda_pdf *document, panda_page *page,
		     char *text, int top, int left, int bottom, int right,
		     int flags)
{
  panda_object *annotation;

  annotation = panda_insertannotation(document, page, top, left, bottom, right,
				      flags);
  
  panda_adddictitem(document, annotation, "Subtype", 
		    panda_textvalue, "FreeText");
  panda_adddictitem(document, annotation, "Contents", 
		    panda_brackettedtextvalue, text);  
  //panda_adddictitem(document, annotation, "DA",
  //		    panda_textvalue, 

  // todo_mikal
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

panda_object *panda_insertannotation(panda_pdf *document, panda_page *page,
				     int top, int left, int bottom, int right,
				     int flags){
  panda_object *annotation;
  char *rectString, *dateString;

  // Each anotation is stored in it's own object because we can
  annotation = panda_newobject(document, panda_normal);
  panda_addchild(document->pages, annotation);

  // Add this to the list of annotations for the named page
  panda_adddictitem(document, page->obj, "Annots",
		    panda_objectarrayvalue, annotation);
  panda_adddictitem(document, annotation, "Type", 
		    panda_textvalue, "Annot");
  panda_adddictitem(document, annotation, "F",
  		    panda_integervalue, flags);
  panda_adddictitem(document, annotation, "M",
  		    panda_brackettedtextvalue, dateString = panda_nowdate());
  panda_xfree(dateString);
  panda_adddictitem(document, annotation, "P",
		    panda_objectvalue, page->obj);

  // The onscreen rectangle
  rectString = panda_xsnprintf("[%d %d %d %d]", left, page->height - bottom, 
			       right, page->height - top);
  panda_adddictitem(document, annotation, "Rect",
		    panda_literaltextvalue, rectString);
  panda_xfree(rectString);

  return annotation;
}
