/******************************************************************************
  trailer.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      19062000
                     Added the %%EOF                                   22062000
		     Added the informational bits to the trailer       03092000

  Purpose:
    Write out the trailer for the PDF.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void
writeTrailer (pdf * output)
{
  // Write out the trailer information for the PDF
  pdfprintf (output,
	     "trailer\n<<\n/Size %d\n/Root %d %d R\n",
	     output->nextObjectNumber + 1,
	     output->catalog->number, output->catalog->generation);

  // If there is an info object, then let it do it's thing
  if (output->info != NULL)
    pdfprintf (output, "/Info %d %d R\n",
	       output->info->number, output->info->generation);

  pdfprintf (output, ">>\nstartxref\n%d\n%sEOF\n", output->xrefOffset, "%%");
}
