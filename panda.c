/******************************************************************************
  Panda.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000
    Michael Still    Added catalog and pages objects to open           17062000
                       and byte offset code
    Michael Still    Added XREF and object traversal code              18062000

  Purpose:
    Library level calls. These are the calls normally accessible to the
    user, and these calls wrapper our calls.
******************************************************************************/

#define  GLOBALS	here
#include "constants.h"
#include "functions.h"

void initpanda(){
  int  generalCounter;

  // We first need to create the binary string to include in our header
  for(generalCounter = 0; generalCounter < 
    (sizeof(gBinaryHeaderString) / sizeof(char)); generalCounter++){
    gBinaryHeaderString[generalCounter] = 
      gBinaryChar(gHeaderString[generalCounter]);
  }
}

pdf *pdfopen(char *filename, char *mode){
  pdf  *openedpdf;

  // We are going to open a PDF for file I/O. Currently, the only supported
  // mode is 'w'. There are some more obscure modes not included in the error
  // checking below (like a+b), deal with it.

  // Make some space for the PDF information
  if((openedpdf = (pdf *) malloc(sizeof(pdf))) == NULL){
    error("Could not create a pdf structure (out of memory?)");
  }

  switch(mode[0]){
  case 'r':
  case 'a':
    error("Unsupported file I/O mode handed to panda.");
    break;

  case 'w':
    // We are opening a new PDF for writing (we hope)
    if(strlen(mode) > 1) switch(mode[1]){
    case '+':
      error("Unsupported file I/O mode handed to panda.");
      break;
    }

    // We _are_ going to create the file
    if((openedpdf->file = fopen(filename, mode)) == NULL)
      return NULL;

    // We have no objects yet
    openedpdf->nextObjectNumber = 1;

    // We are at the begining of the file
    openedpdf->byteOffset = 0;

    // The file will need to have a PDF header to it
    pdfprintf(openedpdf, "%s%s\n", gMagicHeaderString, gBinaryHeaderString);

    // We need a catalog object with some elements within it's dictionary
    openedpdf->catalog = newobject(openedpdf, gNormal);
    adddictitem(openedpdf->catalog->dict, "Type", gTextValue, "Catalog");

    // We need a reference to our pages object
    addchild(openedpdf->catalog, 
      openedpdf->pages = newobject(openedpdf, gNormal));
    adddictitem(openedpdf->catalog->dict, "Pages", gObjValue, openedpdf->pages);

    // We now need to setup some information in the pages object
    adddictitem(openedpdf->pages->dict, "Type", gTextValue, "Pages");
    adddictitem(openedpdf->pages->dict, "Count", gIntValue, 1);

    // Make sure we have not started an XREF table
    openedpdf->xrefTable = NULL;

    // There is no font currently selected
    openedpdf->currentFont = NULL;
    openedpdf->currentFontSize = -1;
    openedpdf->nextFontNumber = 1;

    // The fonts object in the pdf * is a dummy which makes fonts external
    // to each page. This makes the PDF more efficient
    openedpdf->fonts = newobject(openedpdf, gPlaceholder);

    // We did open the PDF file ok
    return openedpdf;
    break;

  default:
    error("Unknown file I/O mode handed to panda.");
    break;
  }

  // How did we get here?
  return NULL;
}

void pdfclose(pdf *openedpdf){
  // We need to write out the objects into the PDF file and then close the
  // file -- any object which heads an object tree, or lives outside the tree
  // structure will need a traverseObjects call here...
  traverseObjects(openedpdf, openedpdf->catalog, writeObject);
  traverseObjects(openedpdf, openedpdf->fonts, writeObject);

  // Write our the XREF object -- this MUST happen after all object have been
  // written, or the byte offsets will not be known
  writeXref(openedpdf);

  // Write the trailer
  writeTrailer(openedpdf);

  fclose(openedpdf->file);
}

page *pdfpage(pdf *output, char *pageSize){
  // Add a page to the PDF
  page  *newPage;

  // Make some space for the object
  if((newPage = (page *) malloc(sizeof(page))) == NULL)
    error("Could not make a new page.");

  // Make the new page object
  newPage->obj = newobject(output, gNormal);

  // Add it to the object tree
  addchild(output->pages, newPage->obj);

  // List it in the Kids field of the pages object
  adddictitem(output->pages->dict, "Kids", gObjArrayValue, newPage->obj);

  // Setup some basic things within the page object's dictionary
  adddictitem(newPage->obj->dict, "Type", gTextValue, "Page");
  adddictitem(newPage->obj->dict, "MediaBox", gLiteralTextValue, pageSize);
  adddictitem(newPage->obj->dict, "Parent", gObjValue, output->pages);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  newPage->contents = newobject(output, gNormal);
  addchild(newPage->obj, newPage->contents);
  adddictitem(newPage->obj->dict, "Contents", gObjValue, newPage->contents);

  return newPage;
}
