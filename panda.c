/******************************************************************************
  Panda.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      08062000
                     Added catalog and pages objects to open           17062000
                       and byte offset code
                     Added XREF and object traversal code              18062000
                     Stored page size in the pages object              30062000

  Purpose:
    Library level calls. These are the calls normally accessible to the
    user, and these calls wrapper our calls.
******************************************************************************/

#define  GLOBALS	here
#include "constants.h"
#include "functions.h"

// Initialise the PDF library ready for operations
void initpanda(){
  int  generalCounter;

  // We first need to create the binary string to include in our header
  for(generalCounter = 0; generalCounter < 
    (sizeof(gBinaryHeaderString) / sizeof(char)); generalCounter++){
    gBinaryHeaderString[generalCounter] = 
      gBinaryChar(gHeaderString[generalCounter]);
  }
}

// Open the named PDF document in the given mode
pdf *pdfopen(char *filename, char *mode){
  pdf  *openedpdf;
  char *tempPtr;

  // We are going to open a PDF for file I/O. Currently, the only supported
  // mode is 'w'. There are some more obscure modes not included in the error
  // checking below (like a+b), deal with it.

  // Make some space for the PDF information
  if((openedpdf = (pdf *) malloc(sizeof(pdf))) == NULL){
    error("Could not create a pdf structure (out of memory?)");
  }

  // Every PDF is going to have to have some xref information associated with
  // it at some stage.
  if((openedpdf->xrefList = (xref *) malloc(sizeof(xref))) == NULL)
    error("Could not make a new xref list for this pdf document.");
  openedpdf->xrefList->next = NULL;
  openedpdf->xrefTail = openedpdf->xrefList;

  // Do stuff based on the mode of the opening
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
    adddictitem(openedpdf->catalog->dict, "Pages", gObjValue,
      openedpdf->pages);

    // We need to remember how many pages there are for later
    openedpdf->pageCount = 0;

    // We now need to setup some information in the pages object
    adddictitem(openedpdf->pages->dict, "Type", gTextValue, "Pages");
    openedpdf->pages->isPages = gTrue;

    // Make sure we have not started an XREF table
    //    openedpdf->xrefTable = NULL;

    // There is no font currently selected
    openedpdf->currentFont = NULL;
    openedpdf->currentFontSize = -1;
    openedpdf->nextFontNumber = 1;

    // The fonts object in the pdf * is a dummy which makes fonts external
    // to each page. This makes the PDF more efficient
    openedpdf->fonts = newobject(openedpdf, gPlaceholder);

    // Set the text mode to something basic
    setfontmode(openedpdf, gTextModeNormal);
    setcharacterspacing(openedpdf, 0.0);
    setwordspacing(openedpdf, 0.0);
    sethorizontalscaling(openedpdf, 1.0);
    setleading(openedpdf, 0.0);

    // Create a dummy object for when we print the pdf to a file
    openedpdf->dummyObj = newobject(openedpdf, gPlaceholder);

    // Setup the info object with some stuff which makes me happy... :)
    openedpdf->info = NULL;
    checkInfo(openedpdf);
    if(openedpdf->info == NULL)
      error("Failed to make an info object for the PDF. Not sure why...");

    // Add some stuff
    adddictitem(openedpdf->info->dict, "Producer", gBracketedTextValue, 
      "Panda 0.2");
    adddictitem(openedpdf->info->dict, "CreationDate", gBracketedTextValue,
      tempPtr = nowdate());
    free(tempPtr);

    // Remember the mode and create the linear object if needed
    if((mode[1] == 'l') || (mode[1] == 'L')){
      openedpdf->mode = gWriteLinear;
      openedpdf->linear = newobject(openedpdf, gNormal);
      adddictitem(openedpdf->linear->dict, "Linearised", gIntValue, 1); 
    }
    else{
      openedpdf->mode = gWrite;
      openedpdf->linear = NULL;
    }

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

// Finish operations on a given PDF document and write the result to disc
void pdfclose(pdf *openedpdf){
  // The header was written when we created the file on disk

  // It is now worth our time to count the number of pages and make the count
  // entry in the pages object
  adddictitem(openedpdf->pages->dict, "Count", gIntValue, 
    openedpdf->pageCount);

  // We do some different things to write out the PDF depending on the mode
  switch(openedpdf->mode){
  case gWrite:
    // We need to write out the objects into the PDF file and then close the
    // file -- any object which heads an object tree, or lives outside the tree
    // structure will need a traverseObjects call here...
    traverseObjects(openedpdf, openedpdf->catalog, gDown, writeObject);
    traverseObjects(openedpdf, openedpdf->fonts, gDown, writeObject);
 
    // Write our the XREF object -- this MUST happen after all objects have 
    // been written, or the byte offsets will not be known
    writeXref(openedpdf);
  
    // Write the trailer
    writeTrailer(openedpdf);
    break;

  case gWriteLinear:
    // Are there any pages in the PDF? There needs to be at least one...
    if(((child *) openedpdf->pages->children)->me == NULL)
      error("Linearised PDFs need at least one page.");

    writeObject(openedpdf, openedpdf->linear);

    // We need the xref entries for the first page now
    
    // And the catalog object
    writeObject(openedpdf, openedpdf->catalog);

    // The primary hint stream

    // We now need all of the objects for the first page
    traverseObjects(openedpdf, 
     ((child *) openedpdf->pages->children)->me, gDown, writeObject);

    
    break;
  }

  // We also need to free all the memory that we no longer need. This is done
  // separately because sometimes we want to write out but not do this
  // in other words I a=inm leaving space for later movement...
  traverseObjects(openedpdf, openedpdf->catalog, gUp, freeObject);
  traverseObjects(openedpdf, openedpdf->fonts, gUp, freeObject);
 
  fclose(openedpdf->file);
}

// Insert a page into the PDF
page *pdfpage(pdf *output, char *pageSize){
  // Add a page to the PDF
  page  *newPage;
  char  *pageSizeCopy;

  // Make some space for the object
  if((newPage = (page *) malloc(sizeof(page))) == NULL)
    error("Could not make a new page.");

  // Make the new page object
  newPage->obj = newobject(output, gNormal);

  // Add it to the object tree
  addchild(output->pages, newPage->obj);

  // List it in the Kids field of the pages object
  // This is now done when the object is written out into the PDF at the end
  // adddictitem(output->pages->dict, "Kids", gObjArrayValue, newPage->obj);

  // Setup some basic things within the page object's dictionary
  adddictitem(newPage->obj->dict, "Type", gTextValue, "Page");
  adddictitem(newPage->obj->dict, "MediaBox", gLiteralTextValue, pageSize);
  adddictitem(newPage->obj->dict, "Parent", gObjValue, output->pages);

  // We also need to do the same sort of thing for the contents object
  // that each page owns
  newPage->contents = newobject(output, gNormal);
  addchild(newPage->obj, newPage->contents);
  adddictitem(newPage->obj->dict, "Contents", gObjValue, newPage->contents);

  // Copy the pageSize string somewhere safe, and then clobber the copy.
  // We can't clober the original because it is a constant anyway and it would
  // be rude to screw with another person's data
  if((pageSizeCopy = (char *) malloc(sizeof(char) * (strlen(pageSize) + 1))) 
    == NULL)
    error("Could not copy the pageSize string.");
  strcpy(pageSizeCopy, pageSize);

  strtok(pageSizeCopy, " ");
  strtok(NULL, " ");
  newPage->width = atoi(strtok(NULL, " "));
  newPage->height = atoi(strtok(NULL, " "));

  // Increment the page count for the PDF
  output->pageCount++;  

  return newPage;
}
