/******************************************************************************
	gnupdf.c

	Change Control:													DDMMYYYY
		Michael Still		File created							17062000

	Purpose:
		Handle all things related to the XREF table at the end of the PDF.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void writeXref(pdf *output){
	xref	*currentXREF;
	int		prevNumber;

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
		output->xrefTable->next = NULL;
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
		if((prevNumber + 1) != currentXREF->number){
			// The next free object is not determined at the moment.
			// I need to fix this. The generation number is 0.
			pdfprintf(output, "%010d %05d f\n", 0, 0);
			prevNumber++;
		}

		pdfprintf(output, "%s", currentXREF->string);
		prevNumber = currentXREF->number;
		currentXREF = currentXREF->next;

		output->xrefCount++;
	}
}

void writeXrefEntry(pdf *output, object *dumpTarget){
	// Add the xref entry into the sorted xrefTable for this PDF
	xref	*currentXREF = output->xrefTable, *temp;

	// Skip placeholders
	if(dumpTarget->number == -1) return;

	// Find the place in the list for this item
	while((currentXREF->next != NULL) && 
		(currentXREF->number < dumpTarget->number)){
		
		currentXREF = currentXREF->next;
	}

	temp = currentXREF->next;

	// Make space for this entry
	if((currentXREF->next = (xref *) malloc(sizeof(xref))) == NULL)
		error("Could not grow the XREF table.");

	currentXREF->number = dumpTarget->number;
	if((currentXREF->string = (char *) malloc(sizeof(char) * 20)) == NULL)
		error("Could not make space for the XREF line.");
	sprintf(currentXREF->string, "%010d %05d n\n",
		dumpTarget->byteOffset,
		dumpTarget->generation);

	currentXREF->next->next = temp;
}