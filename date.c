/******************************************************************************
  date.c

  Change Control:                                                      DDMMYYYY
    Michael Still    Date handling functions first introduced          03092000

  Purpose:
    Functions related to the internal representation of dates in PDF land.

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

#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

#include <time.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_makedate
PURPOSE format a date into the internal PDF representation

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_makedate (int year, int month, int day, int hour, int minutes, int seconds);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. The function makedate generates a date that is suitable to be embedded into a PDF. This date will include information about the timezone in which the PDF was created and the relationship that this timezone has with GMT (Zulu) time. Mostly this function is used internally within Panda, and should not concern the average programmer too much.

RETURNS The date passed formatted as a null terminated string.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

printf("Panda would encode 19 July 1977 3:14:23 AM as: %s\n",
  panda_makedate(1977, 7, 19, 3, 14, 23));

EXAMPLE END
SEEALSO panda_nowdate
DOCBOOK END
******************************************************************************/

// Format a given date into the approved format
char *
panda_makedate (int year, int month, int day, int hour, int minutes,
		int seconds)
{
  char *dateString, zulu = '+';
  int gmthours = 10, gmtminutes = 0;
  time_t curtime;

#if defined _WINDOWS
  struct _timeb tstruct;

  // Setup the timezone in Windows land
  _tzset ();
#else
  struct tm *gmtoffset;
#endif

  // We need to work out how to decide what this machine's relationship to GMT
  // (zulu) so that we can provide this in the date string for the PDF.
  curtime = time (NULL);

  // This is where we actually get the GMT information -- this doesn't work on all
  // operating systems
#if defined _WINDOWS
  // tstruct.timezone is the distance to GMT in minutes
  _ftime (&tstruct);
  gmthours = tstruct.timezone / 60 / 60;
  gmtminutes = (tstruct.timezone - gmthours * 60 * 60) / 60;
#else
  // gmtoffset->tm_gmtoff is the distance to GMT in seconds (linux)
  gmtoffset = (struct tm *) localtime (&curtime);
  gmthours = gmtoffset->tm_gmtoff / 60 / 60;
  gmtminutes = (gmtoffset->tm_gmtoff - gmthours * 60 * 60) / 60;
#endif

  if (gmthours < 0)
    zulu = '-';

  // Do some insurance things to make sure we do y2k dates ok
  if (year < 100)
    {
      if (year > 40)
	year += 1900;
      else
	year += 2000;
    }

  // Make the string
  dateString = panda_xsnprintf ("D:%4d%02d%02d%02d%02d%02d%c%02d'%02d'",
				year, month, day, hour, minutes, seconds,
				zulu, gmthours, gmtminutes);

#if defined DEBUG
  printf ("Returned a date string as requested\n");
#endif

  // And finish
  return dateString;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_nowdate
PURPOSE return the string for now

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_nowdate ();
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. The function nowdate generates a date that is suitable to be embedded into a PDF. This date will include information about the timezone in which the PDF was created and the relationship that this timezone has with GMT (Zulu) time. Mostly this function is used internally within Panda, and should not concern the avaerage programmer too much. This version takes no input, and makes a date for the current time.

RETURNS The current time in PDF format in a null terminated string.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

printf("The time now is %s\n", panda_nowdate());
EXAMPLE END
SEEALSO panda_makedate
DOCBOOK END
******************************************************************************/

// Make a date string for the time that we are currently experiencing
char *
panda_nowdate ()
{
  struct tm *timenow;
  time_t curtime;

#if defined DEBUG
  printf ("Requested the date string for now\n");
#endif

  // Get the current time...
  curtime = time (NULL);
  timenow = (struct tm *) localtime (&curtime);

  return panda_makedate (1900 + timenow->tm_year,
			 timenow->tm_mon + 1,
			 timenow->tm_mday,
			 timenow->tm_hour, timenow->tm_min, timenow->tm_sec);

#if defined DEBUG
  printf ("Returned a date string representing now, as requested\n");
#endif
}
