/******************************************************************************
  font.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062000

  Purpose:
    Routines related to fonts.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_createfont
PURPOSE return a handle to the requested font

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char * panda_createfont (panda_pdf * output, char *fontname, int type, char *encoding)
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This funtion call creates a font object for the requested font and returns the identifier that should be used when the font is set within the PDF document.

RETURNS A font identifier (handle) as a null terminated string.

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;
char *fonthandle;

panda_init();
output = panda_open("output.pdf", "w");
fonthandle = (output, "Helventica", 3, "MacRomanEncoding");
EXAMPLE END
SEEALSO panda_setfont
DOCBOOK END
******************************************************************************/

char *
panda_createfont (panda_pdf * output, char *fontname, int type,
		  char *encoding)
{
  // Create a font object in the PDF
  panda_object *font;
  char *tempBuffer, *fontident;

  // Make the new object
  font = (panda_object *) panda_newobject (output, panda_normal);

  // Add it to the tree of font objects
  panda_addchild (output->fonts, font);

  // Setup some values within the font object
  panda_adddictitem (font->dict, "Type", panda_textvalue, "Font");

  tempBuffer = panda_xsnprintf ("Type%d", type);
  panda_adddictitem (font->dict, "Subtype", panda_textvalue, tempBuffer);

  // Make a font identifier string for this font
  fontident = panda_xsnprintf ("F%08d", output->nextFontNumber);
  output->nextFontNumber++;

  panda_adddictitem (font->dict, "Name", panda_textvalue, fontident);

  panda_adddictitem (font->dict, "BaseFont", panda_textvalue, fontname);
  panda_adddictitem (font->dict, "Encoding", panda_textvalue, encoding);

#if defined DEBUG
  printf ("Returning the font ident \"%s\"\n", fontident);
#endif

  return fontident;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setfont
PURPOSE set the current font to be that specified

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setfont (char *fontident);
SYNOPSIS END

DESCRIPTION Once you have generated a font identifier for a given font, you can then set that current font to that font using this call. Create a font identifier with the <command>panda_createfont</command>() call.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;
char *fonthandle;

panda_init();
output = panda_open ("output.pdf", "w");
fonthandle = panda_createfont (output, "Helventica", 3, "MacRomanEncoding");
panda_setfont (fonthandle);
EXAMPLE END
SEEALSO panda_createfont
DOCBOOK END
******************************************************************************/

void
panda_setfont (panda_pdf * output, char *fontident)
{
  // Free on a NULL should do nothing (check for other platforms)
  if (output->currentFont != NULL)
    free (output->currentFont);

  output->currentFont =
    panda_xmalloc ((strlen (fontident) + 1) * sizeof (char));
  strcpy (output->currentFont, fontident);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setfontsize
PURPOSE set the current font size

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setfontsize (panda_page *target, int size);
SYNOPSIS END

DESCRIPTION Set the size of the font to be used next (in points).

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
setfontsize(output, 42);
EXAMPLE END
SEEALSO panda_setfontmode, panda_setfont
DOCBOOK END
******************************************************************************/

// Set the current font size
void
panda_setfontsize (panda_pdf * output, int size)
{
  output->currentFontSize = size;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_getfontobj
PURPOSE get the object that the named font is stored in

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_getfontobj (panda_pdf *output, char *fontident);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function is used to map a font identifier handed to <command>Panda</command> by a user to the object that actually stores the font requested. This is because the PDF specification actually wants a reference to this object, instead of merely the name of the font.

RETURNS The font object

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");

fonthandle = panda_createfont (output, "Helventica", 3, "MacRomanEncoding");
printf("The font was stored in object number %d\n",
  (panda_getfont (output, fonthandle))->number);
EXAMPLE END
SEEALSO panda_createfont
DOCBOOK END
******************************************************************************/

panda_object *
panda_getfontobj (panda_pdf * output, char *fontident)
{
  // Somewhere there should be a font object with a dictionary key called
  // Name with the value fontident. Find it. We do not handle
  // sub-dictionaries here at the moment...
  panda_child *thisChild;
  panda_dictionary *thisDict;
  char *valueString;

#if defined DEBUG
  printf ("Looking in pdf for font object \"%s\"\n", fontident);
#endif

  // The value string needs to have a / out the front
  valueString = panda_xsnprintf ("/%s", fontident);

  // Start
  thisChild = output->fonts->children;

  // Go through each of the children until we find something
  while (thisChild->next != NULL)
    {
      // We are now going to go through this dictionary
      thisDict = thisChild->me->dict;

      while (thisDict->next != NULL)
	{
	  // Is this the one?
	  if ((strcmp (thisDict->name, "Name") == 0) &&
	      (strcmp (thisDict->textValue, valueString) == 0))
	    return thisChild->me;

	  thisDict = thisDict->next;
	}

      thisChild = thisChild->next;
    }

  // If we got here, then we found nothing
  return NULL;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setfontmode
PURPOSE set the current font mode

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setfontmode (panda_page *target, int mode);
SYNOPSIS END

DESCRIPTION Set the drawing mode for the current font. Valid modes are: panda_textmode_normal, panda_textmode_outline, panda_textmode_filledoutline, panda_textmode_invisible, panda_textmode_filledclipped, panda_textmode_strokedclipped, panda_textmode_filledstrokedclipped and panda_textmode_clipped.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
panda_setfontmode(output, panda_textmode_outline);
EXAMPLE END
SEEALSO panda_setfontsize, panda_setfont
DOCBOOK END
******************************************************************************/

// Set the font mode to something
void
panda_setfontmode (panda_pdf * output, int mode)
{
  output->currentFontMode = mode;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setcharacterspacing
PURPOSE set the space between characters

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setcharacterspacing (panda_page *target, double amount);
SYNOPSIS END

DESCRIPTION Set the amount of additional space between characters in points.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
panda_setcharacterspacing(output, 42.3);
EXAMPLE END
SEEALSO panda_setwordspacing, panda_sethorizontalscaling, panda_setleading
DOCBOOK END
******************************************************************************/

// Set the spacing between characters
void
panda_setcharacterspacing (panda_pdf * output, double amount)
{
  output->currentCharacterSpacing = amount;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setwordspacing
PURPOSE set the space between words

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setwordspacing (panda_page *target, double amount);
SYNOPSIS END

DESCRIPTION Set the amount of additional space between words in points.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
panda_setwordspacing(output, 42.3);
EXAMPLE END
SEEALSO panda_setcharacterspacing, panda_sethorizontalscaling, panda_setleading
DOCBOOK END
******************************************************************************/

// Set the spacing between words
void
panda_setwordspacing (panda_pdf * output, double amount)
{
  output->currentWordSpacing = amount;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_sethorizontalscaling
PURPOSE set the horizontal scaling of text

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_sethorizontalscaling (panda_pdf *output, double amount);
SYNOPSIS END

DESCRIPTION Set the horizontal scaling factor of the text in percent.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
panda_sethorizontalscaling(output, 42.3);
EXAMPLE END
SEEALSO panda_setcharacterspacing, panda_setwordspacing, panda_setleading
DOCBOOK END
******************************************************************************/

// Set the horizontal scaling factor for characters
void
panda_sethorizontalscaling (panda_pdf * output, double scaling)
{
  output->currentHorizontalScaling = scaling;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setleading
PURPOSE set the amount of space between lines of text

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setleading (panda_pdf *output, double amount);
SYNOPSIS END

DESCRIPTION Set the amount of space between lines of text in points.

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *output;

panda_init();
output = panda_open("output.pdf", "w");
panda_setleading(output, 42.3);
EXAMPLE END
SEEALSO panda_setcharacterspacing, panda_setwordspacing, panda_sethorizontalscaling
DOCBOOK END
******************************************************************************/

// Set the spacing between lines
void
panda_setleading (panda_pdf * output, double leading)
{
  output->currentLeading = leading;
}
