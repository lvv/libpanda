/******************************************************************************
  Template.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      10012001

  Purpose:
    PDF allows you to define a template for a set of pages, which contains all
    the elements you can have on a normal page. This makes storage of many,
    mainly identical, pages much more efficient. This is all discussed on page
    264 of the PDF Spec 1.3, second edition.

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

#include <math.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_newtemplate
PURPOSE create a template page in the PDF

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_newtemplate(panda_pdf *document, char *pageSize);
SYNOPSIS END

DESCRIPTION This function is used to create 'template' pages which can then be refered to on other pages. For instance, if you were creating a document that used a standard letter head, then it would make sense to construct the letterhead as a template, and then use this on all the pages. The created template looks and feels just like any other page in the document for the purposes of creating content. Refer to the <command>panda_newpage</command> man page for details on how to use pages.

RETURNS A pointer to the template page

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *templatepage;

panda_init();

document = panda_open("filename.pdf", "w");
templatepage = panda_newtemplate (document, panda_pagesize_a4);
EXAMPLE END
SEEALSO panda_newpage, panda_applytemplate
DOCBOOK END
******************************************************************************/

panda_page *
panda_newtemplate (panda_pdf * output, char *pageSize)
{
  panda_page *template;
  char *pageSizeCopy;

  // Make some space for the object
  template = (panda_page *) panda_xmalloc (sizeof (panda_page));
  template->obj = (panda_object *) panda_newobject (output, panda_normal);
  template->isTemplate = panda_true;

  // Add it to the object tree
  panda_addchild (output->pages, template->obj);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  template->contents = template->obj;
  template->contents->isContents = panda_true;

  // Setup some stuff in the contents object that we need later
  template->contents->textmode = panda_false;
  template->contents->insidegraphicsblock = panda_false;

  // Add the required dictionary elements for a template page
  panda_adddictitem (output, template->obj, "Type", panda_textvalue,
		     "XObject");
  panda_adddictitem (output, template->obj, "Subtype", panda_textvalue,
		     "Form");
  panda_adddictitem (output, template->obj, "FormType", panda_integervalue,
		     1);
  panda_adddictitem (output, template->obj, "BBox", panda_literaltextvalue,
		     pageSize);
  panda_adddictitem (output, template->obj, "Matrix", panda_literaltextvalue,
		     "[1 0 0 1 0 0]");
  panda_adddictitem (output, template->obj, "Resources/ProcSet",
		     panda_literaltextvalue, "[/PDF]");

  // Copy the pageSize string somewhere safe, and then clobber the copy.
  // We can't clobber the original because it is a constant anyway and it would
  // be rude to screw with another person's data
  pageSizeCopy =
    (char *) panda_xmalloc (sizeof (char) * (strlen (pageSize) + 1));
  strcpy (pageSizeCopy, pageSize);

  strtok (pageSizeCopy, " ");
  strtok (NULL, " ");
  template->width = atoi (strtok (NULL, " "));
  template->height = atoi (strtok (NULL, " "));

  panda_xfree (pageSizeCopy);

  return template;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_applytemplate
PURPOSE use a template page previously created

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_applytemplate (panda_pdf * output, panda_page * target,
		     panda_page * template)
SYNOPSIS END

DESCRIPTION This function is used to use a template created with the <command>panda_newtemplate</command> function call.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *templatepage, *realpage;

panda_init();

document = panda_open("filename.pdf", "w");
templatepage = panda_newtemplate (document, panda_pagesize_a4);
realpage = panda_newpage (document, panda_pagesize_a4);

... the order of the drawing commands to the two pages doesn't matter ...

panda_applytemplate(document, realpage, templatepage);
EXAMPLE END
SEEALSO panda_newpage, panda_newtemplate
DOCBOOK END
******************************************************************************/

void
panda_applytemplate (panda_pdf * output, panda_page * target,
		     panda_page * template)
{
  int left = 0, right = 200, top = 0, bottom = 200;
  char *dictkey;

  if (template->isTemplate == panda_false)
    {
      panda_error (panda_false,
		   "Attempt to use a non-template page as a template");
      return;
    }

  // We also need to add some information to the layout stream for the contents
  // object for the page that the image is being displayed on. This information
  // consists of the following [moved to internal.c]
  panda_entergraphicsmode (target);

  target->contents->layoutstream =
    panda_streamprintf (target->contents->layoutstream, "/template-%d Do\n",
			template->obj->number);

  panda_exitgraphicsmode (target);

  dictkey = panda_xsnprintf ("Resources/XObject/template-%d",
			     template->obj->number);
  panda_adddictitem (output, target->obj, dictkey,
		     panda_objectvalue, template->obj);
  panda_xfree (dictkey);
}
