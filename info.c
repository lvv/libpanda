/******************************************************************************
  info.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03092000
                  
  Purpose:
    Build the info object for the document.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

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

// Set the author string for this document
void
panda_setauthor (panda_pdf * document, char *author)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the author bit to it
  panda_adddictitem (document->info->dict, "Author", panda_brackettedtextvalue, author);
}

// Set the creator string for this document
void
panda_setcreator (panda_pdf * document, char *creator)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the creator bit to it
  panda_adddictitem (document->info->dict, "Creator", panda_brackettedtextvalue, creator);
}

// Set the title string for this document
void
panda_settitle (panda_pdf * document, char *title)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the title bit to it
  panda_adddictitem (document->info->dict, "Title", panda_brackettedtextvalue, title);
}

// Set the subject string for this document
void
panda_setsubject (panda_pdf * document, char *subject)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the subject bit to it
  panda_adddictitem (document->info->dict, "Subject", panda_brackettedtextvalue, subject);
}

// Set the keywords string for this document
void
panda_setkeywords (panda_pdf * document, char *keywords)
{
  // Check the info object already exists
  panda_checkinfo (document);

  // Add the keywords bit to it
  panda_adddictitem (document->info->dict, "Keywords", panda_brackettedtextvalue,
	       keywords);
}
