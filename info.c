/******************************************************************************
  info.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03092000
                  
  Purpose:
    Build the info panda_object for the document.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

// Check if an info panda_object exists in the PDF document already
void
panda_checkinfo (panda_pdf * document)
{
  if (document->info == NULL)
    {
      // We need to create an info panda_object
      document->info = panda_newpanda_object (document, gNormal);
      panda_addpanda_child (document->catalog, document->info);
    }
}

// Set the author string for this document
void
panda_setauthor (panda_pdf * document, char *author)
{
  // Check the info panda_object already exists
  panda_checkinfo (document);

  // Add the author bit to it
  panda_adddictitem (document->info->dict, "Author", gBracketedTextValue, author);
}

// Set the creator string for this document
void
panda_setcreator (panda_pdf * document, char *creator)
{
  // Check the info panda_object already exists
  panda_checkinfo (document);

  // Add the creator bit to it
  panda_adddictitem (document->info->dict, "Creator", gBracketedTextValue, creator);
}

// Set the title string for this document
void
panda_settitle (panda_pdf * document, char *title)
{
  // Check the info panda_object already exists
  panda_checkinfo (document);

  // Add the title bit to it
  panda_adddictitem (document->info->dict, "Title", gBracketedTextValue, title);
}

// Set the subject string for this document
void
panda_setsubject (panda_pdf * document, char *subject)
{
  // Check the info panda_object already exists
  panda_checkinfo (document);

  // Add the subject bit to it
  panda_adddictitem (document->info->dict, "Subject", gBracketedTextValue, subject);
}

// Set the keywords string for this document
void
panda_setkeywords (panda_pdf * document, char *keywords)
{
  // Check the info panda_object already exists
  panda_checkinfo (document);

  // Add the keywords bit to it
  panda_adddictitem (document->info->dict, "Keywords", gBracketedTextValue,
	       keywords);
}
