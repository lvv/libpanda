/******************************************************************************
  utility.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      17062000
                     Obscure bug in byte offsets fixed.                22062000
                       Windows printf will add a '\r' even if
                       you don't ask it to, and even if you
                       have added your own. I need to take
                       this into account in the byte offsets.
		     Added textpanda_streamprintf(...) call.           17072000
		     Added putc call                                   22072000

  Purpose:
    Utility functions for the panda library.

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
#define vsnprintf _vsnprintf
  
#else

#include <panda/constants.h>
#include <panda/functions.h>
#endif
  
#include "md5-global.h"
#include "md5.h"
/******************************************************************************
DOCBOOK START

FUNCTION panda_printf
PURPOSE output a formatted string to the PDF file

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_printf (panda_pdf * file, char *format, ...);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call behaves just like <command>printf</command>(), but the output goes to a PDF file, instead of to stdout.

RETURNS Nothing

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
SEEALSO panda_streamprintf, panda_putc, panda_print
DOCBOOK END
******************************************************************************/
// Print a possibly complex string into the PDF file and make sure the offset
// into the PDF file is stored correctly (dispite Windows)
  void
panda_printf (panda_pdf * file, char *format, ...)
{
  char *buffer, *formatString, *token, *strtokVictim;
  int newlineCount = 0, counter = 0, actualLen;
  va_list argPtr;

  // This is a little strange... On a windows machine, printf inserts \r's 
  // for us, even if we have them already. Therefore we don't put them into 
  // the format string, and then add them here if we were not compiled on 
  // windows
#if defined WINDOWS
  formatString =
    (char *) panda_xmalloc ((strlen (format) + 1) * sizeof (char));
  strcpy (formatString, format);
#else
  // We need to go through the format string and replace \n with \r\n
  // because lines in a PDF end with \r\n (one of the options)

  // Count the number of \n's.
  for (counter = 0; counter < strlen (format); counter++)
    if (format[counter] == '\n')
      newlineCount++;

  formatString =
    (char *) panda_xmalloc ((strlen (format) + newlineCount + 1) *
			    sizeof (char));
  formatString[0] = 0;

  // Make every \n a \r\n in the format string
  //  for(counter = indent = 0; counter < strlen(format); counter++){
  //    if(format[counter] == '\n'){
  //      formatString[indent++] = '\r';
  //      formatString[indent++] = format[counter];
  //      }
  //     else formatString[indent++] = format[counter];
  //    }
  //
  //  formatString[indent] = 0;

  strtokVictim =
    (char *) panda_xmalloc (sizeof (char) * (strlen (format) + 1));
  strcpy (strtokVictim, format);
  token = strtok (strtokVictim, "\n");

  // There is a special case when we are at the end of the format string and
  // have a left over token when we don't want to append the newline info...
  while (token != NULL)
    {
      strcat (formatString, token);
      token = strtok (NULL, "\n");

      if (token != NULL)
	strcat (formatString, "\r\n");
    }

  // We don't need strtokVictim any more
  panda_xfree (strtokVictim);

  // We might have missed at \n the end
  if (format[strlen (format) - 1] == '\n')
    strcat (formatString, "\r\n");

#endif

  // Now we need to make a best guess at how long buffer needs to be -- it is 
  // hardly ever longer than 1k...
  buffer = (char *) panda_xmalloc (1024 * sizeof (char));

  // Build the information
  va_start (argPtr, format);
  if ((actualLen = vsnprintf (buffer, 1024, formatString, argPtr)) > 1020)
    {
      // We did not fit! Try again...
      panda_xfree (buffer);

#if defined DEBUG
      printf
	("Needed to make a bigger space for the buffer in panda_printf\n");
#endif

      buffer = (char *) panda_xmalloc (actualLen * sizeof (char));

      if (vsnprintf (buffer, actualLen, formatString, argPtr) > actualLen)
	{
	  panda_error (panda_true, "Really bad file i/o error.");
	}
    }

  // Record how long it was
  file->byteOffset += strlen (buffer);

#if defined WINDOWS
  // Count how many \n's there are and take into account the \r windows will
  // add (change for unix)

  // POSSIBLE BUG: Binary layoutstreams may be corrupted by Windows here
  for (icounter = 0; counter < strlen (buffer); counter++)
    {
      if (buffer[counter] == '\n')
	newlineCount++;
    }

  // Add this number to the byteOffset
  file->byteOffset += newlineCount;
#endif

  // Store it
  fprintf (file->file, "%s", buffer);

  // Free the temp data structures
  panda_xfree (formatString);
  panda_xfree (buffer);
  va_end (argPtr);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_streamprintf
PURPOSE output a formatted string to the text stream associated with a given object

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char *panda_streamprintf (char *stream, char *format, ...);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call behaves just like <command>printf</command>(), but the output goes to a textstream within a PDF object, instead of to stdout. Set the value of the textstream to the value returned...

RETURNS Nothing

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
SEEALSO panda_printf, panda_putc, panda_print
DOCBOOK END
******************************************************************************/

// Append some text to the stream that we are creating for a given page
char *
panda_streamprintf (char *stream, char *format, ...)
{
  va_list argPtr;
  char *buffer;
  unsigned long currentlen = 0, actualLen, len;

  buffer = panda_xmalloc(sizeof(char) * 1024);

  // Get the data to add to the stream
  va_start (argPtr, format);
  if ((actualLen = vsnprintf (buffer, 1024, format, argPtr)) > 1020)
    {
      // We did not fit! Try again...
      panda_xfree (buffer);
      
#if defined DEBUG
      printf
	("Needed to make a bigger space for the buffer in panda_streamprintf\n");
#endif

      buffer = (char *) panda_xmalloc (actualLen * sizeof (char));

      if (vsnprintf (buffer, actualLen, format, argPtr) > actualLen)
	{
	  panda_error (panda_true, "Really bad file i/o error.");
	}
    }
  va_end (argPtr);

  // Make space for the new information
  if (stream != NULL)
    {
      // Print the stream for debugging
#if defined DEBUG
      printf ("-----\nStream is: \"%s\"\n-----\n", stream);
#endif

      // Determine some lengths for the various strings    
      currentlen = strlen (stream) + 1;
      len = strlen (buffer) + 1;

      if ((stream = (char *) realloc (stream,
				      sizeof (char) * (len + currentlen))) ==
	  NULL)
	panda_error
	  (panda_true,
	   "Could not append to an object's stream (of some form).");

      // Do the actual appending
      strncat (stream, buffer, len + currentlen);
    }
  else
    {
      stream = (char *) panda_xmalloc (sizeof (char) * (strlen (buffer) + 1));
      strncpy (stream, buffer, strlen (buffer) + 1);
    }

  // Return the stream
  return stream;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_putc
PURPOSE output a single character to the PDF file

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_printf (panda_pdf * file, char c);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call behaves just like <command>putc</command>(), but the output goes to a PDF file, instead of to stdout.

RETURNS Nothing

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
SEEALSO panda_print, panda_printf, panda_streamprintf
DOCBOOK END
******************************************************************************/

// Put just one character into the PDF file, while updating the offset so that
// the xref table works later on
void
panda_putc (panda_pdf * output, int c)
{
  fputc (c, output->file);
  output->byteOffset++;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_print
PURPOSE output a simple string to the PDF file

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_print (panda_pdf * file, char *chars);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call behaves just like <command>putc</command>(), but the output goes to a PDF file, instead of to stdout. It also writes more than one character at a time to the file, which is faster than doing this with a panda_printf.

RETURNS Nothing

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
SEEALSO panda_putc, panda_printf, panda_streamprintf
DOCBOOK END
******************************************************************************/

// Put some known text into the PDF file, also update the offset
// Don't forget to do the \n to \r\n conversion as well
void
panda_print (panda_pdf * output, char *format)
{
  char *strtokVictim, *formatString, *token;
  int counter, newlineCount;

#if defined WINDOWS
  formatString =
    (char *) panda_xmalloc ((strlen (format) + 1) * sizeof (char));
  strcpy (formatString, format);
#else
  // We need to go through the format string and replace \n with \r\n
  // because lines in a PDF end with \r\n (one of the options)

  // Count the number of \n's.
  newlineCount = 0;
  for (counter = 0; counter < strlen (format); counter++)
    if (format[counter] == '\n')
      newlineCount++;

  formatString =
    (char *) panda_xmalloc ((strlen (format) + newlineCount + 1) *
			    sizeof (char));
  formatString[0] = 0;

  strtokVictim =
    (char *) panda_xmalloc (sizeof (char) * (strlen (format) + 1));
  strcpy (strtokVictim, format);
  token = strtok (strtokVictim, "\n");

  // There is a special case when we are at the end of the format string and
  // have a left over token when we don't want to append the newline info...
  while (token != NULL)
    {
      strcat (formatString, token);
      token = strtok (NULL, "\n");

      if (token != NULL)
	strcat (formatString, "\r\n");
    }

  // We might have missed at \n the end
  if (format[strlen (format) - 1] == '\n')
    strcat (formatString, "\r\n");

  // We don't need strtokVictim any more
  panda_xfree (strtokVictim);
#endif

  fprintf (output->file, "%s", formatString);
  output->byteOffset += strlen (formatString);

  // With windows we might also need to update the newline count here

  // Clean up
  panda_xfree (formatString);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_xsnprintf
PURPOSE a safe version of vnsprintf

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char *panda_xsnprintf(const char* format, va_list ap);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This command behaves like a slightly modified version of <command>vsnprintf</command>. The main difference is that if catches the various different ways that errors can be returned, and turns these into a single NULL is a string big enough to hold the formatted string could not be allocated. 

RETURNS A string

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
DOCBOOK END
******************************************************************************/

char *
panda_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = panda_xrealloc (output, size);
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
	{
	  /* Up to glibc 2.0.6 and Microsoft's implementation */
	  size += 100;
	}
      else
	{
	  /* Check if we are done */
	  if (result < size)
	    break;

	  /* Glibc from now on */
	  size = result + 1;
	}
    }

  va_end (ap);
  return output;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_md5hash
PURPOSE a wrapper to Rivest's MD5 code

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char *panda_md5hash(char *input);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function generates a hash on the given string and then returns that hash.

RETURNS A hash on the input string as a string

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
DOCBOOK END
******************************************************************************/

char *
panda_md5hash (char *input)
{
  MD5_CTX context;
  unsigned char *digest;

  digest = panda_xmalloc (16);

  MD5Init (&context);
  MD5Update (&context, input, strlen (input));
  MD5Final (digest, &context);

  return digest;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_hexstring
PURPOSE take a string of possibly binary bytes, and represent them in hex

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char *panda_hexstring(char *input);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function converts a binary array of chars into an array of hex characters. An assumption is made that there are no zero bytes in the middle of the string.

RETURNS A string of hex

EXAMPLE START
This is an internal function which will only be needed by those playing deeply with Panda itself, so I won't provide an example.
EXAMPLE END
DOCBOOK END
******************************************************************************/

char *
panda_hexstring (char *input)
{
  int length, count;
  char *output;

  length = strlen (input);
  output = panda_xmalloc ((length + 1) * sizeof (char));
  
    // todo_mikal: not sure I like the windows version of this code
    for (count = 0; count < length; count++)
    {
#if defined _WINDOWS
      sprintf (output[count], "%2x", input[count]);
      
#else	/*  */
      snprintf (output[count], 1, "%2x", input[count]);
      
#endif
    }

  output[length + 1] = 0;
  return output;
}
