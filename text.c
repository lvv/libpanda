/******************************************************************************
  text.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062000
                     Improvements to text handling                     30062000

  Purpose:
    Functions related to displaying text on a PDF page.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void
textbox (pdf * output, page * thisPage, int top, int left, int bottom,
	 int right, char *text)
{
  // Add a box with some text in it into the PDF page
  object *textobj;
  char *currentToken, *strtokVictim = NULL, delim[10];
  int internalTop, internalLeft, displayedFirstPart = gFalse;
  object *subdict, *subsubdict, *fontObj;

  /***************************************************************************
     Some text handling
  ***************************************************************************/

  // Make an object to store the text
  //  textobj = newobject(output, gNormal);
  //  addchild(thisPage->obj, textobj);
  //  adddictitem(thisPage->obj->dict, "Contents", gObjValue, textobj);
  textobj = thisPage->contents;

  // Is there a font setup? Does this work with changing the font?
  if (output->currentFont == NULL)
    setfont (output, createfont (output, "Helvetica", 1, "MacRomanEncoding"));

  // If the font is not defined on this page
  if (thisPage->obj->currentSetFont == NULL)
    {
    /************************************************************************
    This font has to be referred to in the resources entry in the dictionary.

    The resources dictionary looks a lot like:
      /Resources <<
        /Font <<
          /...Name... ...Obj Reference...
          >>
        >>

    An example line is /MikalsFont 54 0 R

    This does not currently allow more than one font per page... Doh!
    *************************************************************************/

      // Find the font object needed
#if defined DEBUG
      printf("Searching for the font object relating to \"%s\"\n",
             output->currentFont);
#endif
      if ((fontObj = getfontobj (output, output->currentFont)) == NULL)
	error ("Could not find the font requested.");

      // We make an object not just a dictionary because this is what
      // adddictitem needs
      subsubdict = newobject (output, gPlaceholder);
      adddictitem (subsubdict->dict, output->currentFont, gObjValue, fontObj);

      subdict = newobject (output, gPlaceholder);
      adddictitem (subdict->dict, "Font", gDictionaryValue, subsubdict->dict);

      // And put this into the PDF
      adddictitem (thisPage->obj->dict, "Resources", gDictionaryValue,
		   subdict->dict);
    }

  // Is there a text size setup?
  if (output->currentFontSize == -1)
    setfontsize (output, 16);

  /***************************************************************************
    PDF deals in points, with the bottom left hand side of the page being at
    0,0. I think this is anti-intuitive for most users, so I convert to that
    coordinate system from a normal one here. I define the point 0,0 as being
    the top left of the page. Spacing is still done in points though...

    Further more, the specification for PDF requires that you give the
    location of the bottom of the line of text, not the top. We change this
    my playing games with the known height of the line.
  ***************************************************************************/

  // We know the width and height from the page object
  internalTop = thisPage->height - top - output->currentFontSize;
  internalLeft = left;

  /***************************************************************************
    Back to inserting the text
  ***************************************************************************/

  // Build the command buffer
  // This is a little dodgy at the moment because I do not understand the
  // PS matrix environment well enough to be writing this sort of code. I am
  // going to have to have a look into this a little more...
  textobj->textstream =
    streamprintf (textobj->textstream, "1 0 0 1 %d %d Tm\n", internalLeft,
		  internalTop);

  // There are now a whole bunch of options that may or may not need to be set
  if (output->currentFontMode != 0)
    {
      textobj->textstream = streamprintf (textobj->textstream, "%d Tr\n",
					  output->currentFontMode);
    }

  if (output->currentCharacterSpacing != 0)
    {
      textobj->textstream = streamprintf (textobj->textstream, "%.2f Tc\n",
					  output->currentCharacterSpacing);
    }

  if (output->currentWordSpacing != 0)
    {
      textobj->textstream = streamprintf (textobj->textstream, "%.2f Tw\n",
					  output->currentWordSpacing);
    }

  if (output->currentHorizontalScaling != 1)
    {
      textobj->textstream = streamprintf (textobj->textstream, "%.0f Tz\n",
					  output->currentHorizontalScaling *
					  100);
    }

  if (output->currentLeading != 0)
    {
      textobj->textstream = streamprintf (textobj->textstream, "%.2f TL\n",
					  output->currentLeading);
    }

  // If the font that we are using on the page is not the font that is
  // currently set, then the font has changed and we will need to define the
  // font here
  if ((textobj->currentSetFont == NULL) ||
      (strcmp (output->currentFont, textobj->currentSetFont) != 0))
    {
      // Set the font that we want to use
      textobj->textstream = streamprintf (textobj->textstream, "/%s %d Tf\n",
					  output->currentFont,
					  output->currentFontSize);

      // Make space for the new name
      if (
	  (textobj->currentSetFont =
	   malloc (sizeof (char) * strlen (output->currentFont) + 1)) == NULL)
	error ("Could not copy the font to the new page.");

      // Store the name so we know what is happening
      strcpy (textobj->currentSetFont, output->currentFont);
    }

  /***************************************************************************
    PDFs are funny in that we have to specify where line breaks are to occur.
    We allow the programmer to specify line breaks by putting the characters
    '\n' into the string that we is having us put into the PDF. We therefore
    need to find this character sequence and handle it...

                        ---------------------------

    We also now support super and subscript text options, which means we need
    a portable way for the programmer to express these to us. I have decided
    to go with a scheme in which the character sequence:
           \4U<<>>

    means superscript by the amount which exists at <<>>, which is a one digit
    number expressed in ASCII

    And the sequence:
	   \5D<<>>

    means subscript by the amount passed.

    Finally, we need to be able to get back into normal mode, which is done
    with a:
           \6N
  ***************************************************************************/

  // Get the first token
  if ((strtokVictim = (char *) malloc (sizeof (char) * strlen (text) + 1)) ==
      NULL)
    error ("Could not make space for temporary copy of textbox text.");
  strcpy (strtokVictim, text);

  // Build the delimiter string
  sprintf (delim, "\n%c%c%c", 4, 5, 6);

  currentToken = strtok (strtokVictim, delim);

  while (currentToken != NULL)
    {
      // If we haven't displayed that first part that would otherwise be missed
      // do so now
      if (displayedFirstPart == gFalse)
	{
	  textobj->textstream = streamprintf (textobj->textstream, "(%s) '\n",
					      strtokVictim);
	  displayedFirstPart = gTrue;
	}

      switch (text[currentToken - strtokVictim - 1])
	{
	case '\n':
	  textobj->textstream = streamprintf (textobj->textstream, "(%s) '\n",
					      currentToken);
	  break;

	case 4:
	  textobj->textstream = streamprintf (textobj->textstream,
					      "%c Ts (%s) Tj\n",
					      currentToken[0],
					      currentToken + 1);
	  break;

	case 5:
	  textobj->textstream = streamprintf (textobj->textstream,
					      "-%c Ts (%s) Tj\n",
					      currentToken[0],
					      currentToken + 1);
	  break;

	case 6:
	  textobj->textstream = streamprintf (textobj->textstream,
					      "0 Ts (%s) Tj\n", currentToken);
	  break;
	}

      currentToken = strtok (NULL, delim);
    }

  // Free temp data
  free (strtokVictim);
}
