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
createfont (pdf * output, char *fontname, int type, char *encoding)
{
  // Create a font object in the PDF
  object *font;
  char tempBuffer[10], *fontident;

  // Make the new object
  font = newobject (output, gNormal);

  // Add it to the tree of font objects
  addchild (output->fonts, font);

  // Setup some values within the font object
  adddictitem (font->dict, "Type", gTextValue, "Font");

  sprintf (tempBuffer, "Type%d", type);
  adddictitem (font->dict, "Subtype", gTextValue, tempBuffer);

  // Make a font identifier string for this font
  fontident = xmalloc(10 * sizeof (char));
  sprintf (fontident, "F%08d", output->nextFontNumber);
  output->nextFontNumber++;

  adddictitem (font->dict, "Name", gTextValue, fontident);

  adddictitem (font->dict, "BaseFont", gTextValue, fontname);
  adddictitem (font->dict, "Encoding", gTextValue, encoding);

#if defined DEBUG
  printf("Returning the font ident \"%s\"\n", fontident);
#endif

  return fontident;
}

void
setfont (pdf * output, char *fontident)
{
  // Free on a NULL should do nothing (check for other platforms)
  free(output->currentFont);

  output->currentFont = xmalloc((strlen(fontident) + 1) * sizeof(char));
  strcpy(output->currentFont, fontident);
}

// Set the current font size
void
setfontsize (pdf * output, int size)
{
  output->currentFontSize = size;
}

object *
getfontobj (pdf * output, char *fontident)
{
  // Somewhere there should be a font object with a dictionary key called
  // Name with the value fontident. Find it. We do not handle
  // sub-dictionaries here at the moment...
  child *thisChild;
  dictionary *thisDict;
  char valueString[20];

#if defined DEBUG
  printf("Looking in pdf for font object \"%s\"\n", fontident);
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
setfontmode (pdf * output, int mode)
{
  output->currentFontMode = mode;
}

// Set the spacing between characters
void
setcharacterspacing (pdf * output, double amount)
{
  output->currentCharacterSpacing = amount;
}

// Set the spacing between words
void
setwordspacing (pdf * output, double amount)
{
  output->currentWordSpacing = amount;
}

// Set the horizontal scaling factor for characters
void
sethorizontalscaling (pdf * output, double scaling)
{
  output->currentHorizontalScaling = scaling;
}

// Set the spacing between lines
void
setleading (pdf * output, double leading)
{
  output->currentLeading = leading;
}
