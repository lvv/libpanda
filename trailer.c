/******************************************************************************
	trailer.c

	Change Control:													DDMMYYYY
		Michael Still		File created							19062000
		Michael Still		Added the %%EOF							22062000

	Purpose:
		Write out the trailer for the PDF.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void writeTrailer(pdf *output){
	// Write out the trailer information for the PDF
	pdfprintf(output, "trailer\n<<\n/Size %d\n/Root %d %d R\n>>\nstartxref\n%d\n%sEOF",
		output->xrefCount + 1,
		output->catalog->number, output->catalog->generation,
		output->xrefOffset,
		"%%");
}