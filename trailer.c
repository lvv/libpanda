/******************************************************************************
  trailer.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      19062000
                     Added the %%EOF                                   22062000
		     Added the informational bits to the trailer       03092000

  Purpose:
    Write out the trailer for the PDF.

  Copyright (C) Michael Still 2000 - 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#if defined _WINDOWS
#include "stdafx.h"
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

/******************************************************************************
DOCBOOK START

FUNCTION panda_writetrailer
PURPOSE write out the trailer for the PDF document

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_writetrailer (panda_pdf *output);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This command is used to write out the trailer of the PDF document, which contains information about the PDF document, including a pointer to the XREF table.

RETURNS Nothing

EXAMPLE START
Why would you need an example?
EXAMPLE END
DOCBOOK END
******************************************************************************/

void
panda_writetrailer (panda_pdf * output)
{
  // Write out the trailer information for the PDF
  panda_printf (output,
		"trailer\n<<\n/Size %d\n/Root %d %d R\n",
		output->nextObjectNumber + 1,
		output->catalog->number, output->catalog->generation);

  // If there is an info object, then let it do it's thing
  if (output->info != NULL)
    panda_printf (output, "/Info %d %d R\n",
		  output->info->number, output->info->generation);

  panda_printf (output, ">>\nstartxref\n%d\n%sEOF\n", output->xrefOffset,
		"%%");
}
