/******************************************************************************
  panda.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      17062000
                     Added debug option                                03072000

  Purpose:
    Handle all things related to the XREF table at the end of the PDF.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void writeXref(pdf *output){
  xref  *currentXREF;
  int  prevNumber;

  // Save the offset of the xref section for the trailer
  output->xrefOffset = output->byteOffset;
  output->xrefCount = 0;

  // We need to write out the XREF table, the object number of the catalog
  // object is the lowest and %d is the object count. The first line is a 
  // magic incantation
  pdfprintf(output, "xref\n%d %d\n%010d 65535 f\n",
    0,
    output->nextObjectNumber, 0);

  // Initialise the XREF table data in the PDF if it is not already
  if(output->xrefTable == NULL){
    if((output->xrefTable = (xref *) malloc(sizeof(xref))) == NULL)
      error("Could not initiailise the XREF table while writing out.");

    output->xrefTable->number = -42;

    if((output->xrefTable->next = (xref *) malloc(sizeof(xref))) == NULL)
      error("Could not initiailise the XREF table while writing out (2)."); 

    output->xrefTable->next->next = NULL;
  }

  // All of the object entries are inuse, because I see no point in creating
  // free objects at this point. We need to go through the object tree and
  // find all the objects, and make sure they are listed in order in the xref
  // table. All tree heads and objects outside the tree will need a 
  // traverseObjects call here.
  traverseObjects(output, output->catalog, writeXrefEntry);
  traverseObjects(output, output->fonts, writeXrefEntry);

  // Output the XREF table
  currentXREF = output->xrefTable;
  prevNumber = output->xrefTable->number - 1;

  while(currentXREF->next != NULL){
#if defined DEBUG
    printf("Writing XREF entry for object %d\n", currentXREF->number);
#endif

    if(currentXREF->number == -42){
      // We skip these
      prevNumber = currentXREF->next->number - 1;
    }
    else if((prevNumber + 1) != currentXREF->number){
      // The next free object is not determined at the moment.
      // I need to fix this. The generation number is 0.
#if defined DEBUG
      pdfprintf(output, "<<%3d Free>> ", currentXREF->number);
#endif
      pdfprintf(output, "%010d %05d f\n", 0, 0);
      prevNumber = currentXREF->number;
    }
    else{
#if defined DEBUG

      pdfprintf(output, "<<%3d Full>> ", currentXREF->number);
#endif
      pdfprintf(output, "%s\n", currentXREF->string);
      prevNumber = currentXREF->number;
      }
    
    // Move onto the next entry
    currentXREF = currentXREF->next;
    output->xrefCount++;
  }
}

void writeXrefEntry(pdf *output, object *dumpTarget){
  // Add the xref entry into the sorted xrefTable for this PDF
  xref  *currentXREF = output->xrefTable, *temp, *previous;

#if defined DEBUG
  printf("Collecting XREF information for object %d\n", dumpTarget->number);
#endif

  // Skip placeholders
  if(dumpTarget->number == -1) return;

  // We start on the second one in the list
  previous = currentXREF;
  currentXREF = currentXREF->next;

  // Find the place in the list for this item
  while((currentXREF->next != NULL) && 
    (currentXREF->number < dumpTarget->number)){
    previous = currentXREF;
    currentXREF = currentXREF->next;
  }

  // We always go one too far, so back up
  currentXREF = previous;

  /***************************************************************************
    Add the entry, either as an append or as an insertion.
  ***************************************************************************/

  temp = currentXREF->next;

  // Make space for this entry
  if((currentXREF->next = (xref *) malloc(sizeof(xref))) == NULL)
    error("Could not grow the XREF table.");

  // Move into it
  //  currentXREF = currentXREF->next;

  currentXREF->next->number = dumpTarget->number;
  if((currentXREF->next->string = (char *) malloc(sizeof(char) * 20)) == NULL)
    error("Could not make space for the XREF line.");

  sprintf(currentXREF->next->string, "%010ld %05d n",
    dumpTarget->byteOffset,
    dumpTarget->generation);

  currentXREF->next->next = temp;
}

