/******************************************************************************
  info.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03092000
                  
  Purpose:
    Build the info object for the document.
******************************************************************************/

#include "constants.h"
#include "functions.h"

// Check if an info object exists in the PDF document already
void checkInfo(pdf *document){
  if(document->info == NULL){
    // We need to create an info object
    document->info = newobject(document, gNormal);
    addchild(document->pages, document->info);
  }
}

// Set the author string for this document
void setauthor(pdf *document, char *author){
  // Check the info object already exists
  checkInfo(document);

  // Add the author bit to it
  adddictitem(document->info->dict, "Author", gBracketedTextValue, author);
}

// Set the creator string for this document
void setcreator(pdf *document, char *creator){
  // Check the info object already exists
  checkInfo(document);

  // Add the creator bit to it
  adddictitem(document->info->dict, "Creator", gBracketedTextValue, creator);
}

// Set the title string for this document
void settitle(pdf *document, char *title){
  // Check the info object already exists
  checkInfo(document);

  // Add the title bit to it
  adddictitem(document->info->dict, "Title", gBracketedTextValue, title);
}

// Set the subject string for this document
void setsubject(pdf *document, char *subject){
  // Check the info object already exists
  checkInfo(document);

  // Add the subject bit to it
  adddictitem(document->info->dict, "Subject", gBracketedTextValue, subject);
}

// Set the keywords string for this document
void setkeywords(pdf *document, char *keywords){
  // Check the info object already exists
  checkInfo(document);

  // Add the keywords bit to it
  adddictitem(document->info->dict, "Keywords", gBracketedTextValue, keywords);
}
