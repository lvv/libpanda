/******************************************************************************
  info.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03092000
                  
  Purpose:
    Build the info object for the document.

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

FUNCTION panda_checkinfo
PURPOSE ensure an info object exists

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_checkinfo (panda_pdf *output);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call ensures that the info object exists inside the PDF document so that information may be safely added. This function call is unlikely to be needed by users of the API because it is used whenever one of the info object functions is called by the <command>Panda</command> code itself.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_checkinfo(document);
EXAMPLE END
SEEALSO panda_setauthor, panda_setcreator, panda_settitle, panda_setsubject, panda_setkeywords, panda_setid
DOCBOOK END
******************************************************************************/

// Check if an info object exists in the PDF document already
void
panda_checkinfo (panda_pdf * document)
{
  if (document->info == NULL)
    {
      // We need to create an info object
      document->info = panda_newobject (document, panda_normal);
      panda_addchild (document->catalog, document->info);
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setauthor
PURPOSE set the author string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setauthor (panda_pdf *output, char *value);
SYNOPSIS END

DESCRIPTION This function sets the value of the author within the PDF document.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_setauthor(document, "Mikal");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setcreator, panda_settitle, panda_setsubject, panda_setkeywords, panda_setid
DOCBOOK END
******************************************************************************/

// Set the author string for this document
void
panda_setauthor (panda_pdf * document, char *author)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the author bit to it
  panda_adddictitem (document, document->info, "Author",
		     panda_brackettedtextvalue, author);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setcreator
PURPOSE set the creator string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setcreator (panda_pdf *output, char *value);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function sets the value of the creator within the PDF document. This is normally the string "Panda", but may be overridden by another call to the function by a user of the API. I'm not sure why you would want to do that though...

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_setcreator(document, "Panda PDF Generator");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setauthor, panda_settitle, panda_setsubject, panda_setkeywords, panda_setid
DOCBOOK END
******************************************************************************/

// Set the creator string for this document
void
panda_setcreator (panda_pdf * document, char *creator)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the creator bit to it
  panda_adddictitem (document, document->info, "Creator",
		     panda_brackettedtextvalue, creator);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_settitle
PURPOSE set the title string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_settitle (panda_pdf *output, char *value);
SYNOPSIS END

DESCRIPTION This function sets the value of the title within the PDF document.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_settitle(document, "Mikal's excellent PDF document");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setcreator, panda_setauthor, panda_setsubject, panda_setkeywords, panda_setid
DOCBOOK END
******************************************************************************/

// Set the title string for this document
void
panda_settitle (panda_pdf * document, char *title)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the title bit to it
  panda_adddictitem (document, document->info, "Title",
		     panda_brackettedtextvalue, title);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setsubject
PURPOSE set the subject string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setsubject (panda_pdf *output, char *value);
SYNOPSIS END

DESCRIPTION This function sets the value of the subject within the PDF document.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_setsubject(document, "Mikal's homework");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setauthor, panda_setcreator, panda_settitle, panda_setkeywords, panda_setid
DOCBOOK END
******************************************************************************/

// Set the subject string for this document
void
panda_setsubject (panda_pdf * document, char *subject)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the subject bit to it
  panda_adddictitem (document, document->info, "Subject",
		     panda_brackettedtextvalue, subject);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setkeywords
PURPOSE set the keywords string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setkeywords (panda_pdf *output, char *value);
SYNOPSIS END

DESCRIPTION This function sets the value of the keywords string within the PDF document. The string is merely a list of keywords in the form "cats dogs hamsters chickens"

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_setkeywords(document, "panda documentation pdf api generate");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setauthor, panda_setcreator, panda_settitle, panda_setsubject, panda_setid
DOCBOOK END
******************************************************************************/

// Set the keywords string for this document
void
panda_setkeywords (panda_pdf * document, char *keywords)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the keywords bit to it
  panda_adddictitem (document, document->info, "Keywords",
		     panda_brackettedtextvalue, keywords);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setid
PURPOSE set the id string for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setid (panda_pdf *output, char *filename);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function sets the value of the document id within the PDF document. This is based on an MD5 hash of the file creation time, and the file path. Adobe products use some other stuff, but this should be sufficient to have a unique hash generated.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");
panda_setid(document, "filename.pdf");
EXAMPLE END
SEEALSO panda_checkinfo, panda_setauthor, panda_setcreator, panda_settitle, panda_setsubject, panda_setkeywords
DOCBOOK END
******************************************************************************/

void
panda_setid (panda_pdf * document, char *filename)
{
  char *now, *input, *hash, *version, *hexHash, *hexVersion, *totalString;

  // Check the info object already exists
  panda_checkinfo (document);

  now = panda_nowdate ();
  input = panda_xsnprintf ("%s-%s", now, filename);
  hash = panda_md5hash (input);
  version = panda_md5hash ("001");
  hexHash = panda_hexstring (hash);
  hexVersion = panda_hexstring (version);
  totalString = panda_xsnprintf ("[<%s><%s>]", hexHash, hexVersion);

  // Add the ID bit to the dictionary
  panda_adddictitem (document, document->info, "ID",
		     panda_literaltextvalue, totalString);

  panda_xfree (totalString);
  panda_xfree (hexHash);
  panda_xfree (hexVersion);
  panda_xfree (hash);
  panda_xfree (version);
  panda_xfree (input);
  panda_xfree (now);
}
