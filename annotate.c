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

void
panda_textannotation(panda_pdf *document, panda_page *page, int open,
		     char *text, int top, int left, int bottom, int right,
		     int icon, int flags)
{
  panda_object *annotation;

  annotation = panda_insertannotation(document, page, top, left, bottom, right,
				      icon, flags);
  
  panda_adddictitem(document, annotation, "Open",
		    panda_booleanvalue, open);
  panda_adddictitem(document, annotation, "Subtype", 
		    panda_textvalue, "Text");
  panda_adddictitem(document, annotation, "Contents", 
		    panda_brackettedtextvalue, text);  
}

// Internal call
panda_object *panda_insertannotation(panda_pdf *document, panda_page *page,
				     int top, int left, int bottom, int right,
				     int icon, int flags){
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

  // The onscreen rectangle
  rectString = panda_xsnprintf("[%d %d %d %d]", left, page->height - bottom, 
			       right, page->height - top);
  panda_adddictitem(document, annotation, "Rect",
		    panda_literaltextvalue, rectString);
  panda_xfree(rectString);

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

  return annotation;
}
