/******************************************************************************
  xref.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      17062000
                     Added debug option                                03072000

  Purpose:
    Handle all things related to the XREF table at the end of the PDF.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_writexref
PURPOSE write out the table that lists object locations in the file

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_writexref (panda_pdf * output);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Write out the XREF table in the PDF file that we have just generated.

RETURNS Nothing

EXAMPLE START
This command is internal to Panda, and you shouldn't ever need to call it...
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_writexref (panda_pdf * output)
{
  panda_xref *currentXREF;

  // Save the offset of the xref section for the trailer
  output->xrefOffset = output->byteOffset;

  // We need to write out the XREF table, the object number of the catalog
  // object is the lowest and %d is the object count. The first line is a 
  // magic incantation
  panda_printf (output, "xref\n%d %d\n%010d 65535 f\n",
		0, output->nextObjectNumber, 0);

  // All of the object entries are inuse, because I see no point in creating
  // free objects at this point.

  // Output the XREF table -- this is now a simple linked list traversal
  currentXREF = output->xrefList;

  while (currentXREF->next != NULL)
    {
      panda_printf (output, "%010d %05d n\n", currentXREF->this->byteOffset,
		    0);
      currentXREF = currentXREF->next;
    }
}
