/******************************************************************************
  font.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062000

  Purpose:
    Routines related to fonts.
******************************************************************************/

#include <panda/constants.h>
#include <panda/functions.h>

char *
panda_createfont (panda_pdf * output, char *fontname, int type,
		  char *encoding)
{
  // Create a font object in the PDF
  panda_object *font;
  char tempBuffer[10], *fontident;

  // Make the new object
  font = panda_newobject (output, panda_normal);

  // Add it to the tree of font objects
  panda_addchild (output->fonts, font);

  // Setup some values within the font object
  panda_adddictitem (font->dict, "Type", panda_textvalue, "Font");

  sprintf (tempBuffer, "Type%d", type);
  panda_adddictitem (font->dict, "Subtype", panda_textvalue, tempBuffer);

  // Make a font identifier string for this font
  fontident = panda_xmalloc (10 * sizeof (char));
  sprintf (fontident, "F%08d", output->nextFontNumber);
  output->nextFontNumber++;

  panda_adddictitem (font->dict, "Name", panda_textvalue, fontident);

  panda_adddictitem (font->dict, "BaseFont", panda_textvalue, fontname);
  panda_adddictitem (font->dict, "Encoding", panda_textvalue, encoding);

#if defined DEBUG
  printf ("Returning the font ident \"%s\"\n", fontident);
#endif

  return fontident;
}

void
panda_setfont (panda_pdf * output, char *fontident)
{
  // Free on a NULL should do nothing (check for other platforms)
  free (output->currentFont);

  output->currentFont =
    panda_xmalloc ((strlen (fontident) + 1) * sizeof (char));
  strcpy (output->currentFont, fontident);
}

// Set the current font size
void
panda_setfontsize (panda_pdf * output, int size)
{
  output->currentFontSize = size;
}

panda_object *
panda_getfontobj (panda_pdf * output, char *fontident)
{
  // Somewhere there should be a font object with a dictionary key called
  // Name with the value fontident. Find it. We do not handle
  // sub-dictionaries here at the moment...
  panda_child *thisChild;
  panda_dictionary *thisDict;
  char valueString[20];

#if defined DEBUG
  printf ("Looking in pdf for font object \"%s\"\n", fontident);
#endif

  // The value string needs to have a / out the front
  sprintf (valueString, "/%s", fontident);

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

// Set the font mode to something
void
panda_setfontmode (panda_pdf * output, int mode)
{
  output->currentFontMode = mode;
}

// Set the spacing between characters
void
panda_setcharacterspacing (panda_pdf * output, double amount)
{
  output->currentCharacterSpacing = amount;
}

// Set the spacing between words
void
panda_setwordspacing (panda_pdf * output, double amount)
{
  output->currentWordSpacing = amount;
}

// Set the horizontal scaling factor for characters
void
panda_sethorizontalscaling (panda_pdf * output, double scaling)
{
  output->currentHorizontalScaling = scaling;
}

// Set the spacing between lines
void
panda_setleading (panda_pdf * output, double leading)
{
  output->currentLeading = leading;
}
