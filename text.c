/******************************************************************************
  text.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062000

  Purpose:
    Functions related to displaying text on a PDF page.
******************************************************************************/

#if defined _WINDOWS
  #include "panda/constants.h"
  #include "panda/functions.h"
#else
  #include <panda/constants.h>
  #include <panda/functions.h>
#endif

#include <math.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_textbox
PURPOSE display some text on the PDF page specified

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_textbox (panda_pdf * output, panda_page * thisPage, int top, int left, int bottom, int right, char *text);
SYNOPSIS END

DESCRIPTION This function call creates a textbox on the specified page, and then displays the specified text within that page. The current font mode and style et cetera will be used. Sometime in the near future, line wrapping will be used...

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
panda_textbox (demo, currPage, 20 + (lineDepth * 20), 200, 40 + (lineDepth * 20), 400, "Demonstration of a text mode");
EXAMPLE END
SEEALSO panda_createfont, panda_setfont, panda_panda_setfontsize, panda_getfontobj, panda_setfontmode, panda_setcharacterspacing, panda_setwordspacing, panda_sethorizontalscaling, panda_setleading, panda_textboxrot
DOCBOOK END
******************************************************************************/

void
panda_textbox (panda_pdf * output, panda_page * thisPage, int top, int left,
	       int bottom, int right, char *text)
{
  panda_textboxrot (output, thisPage, top, left, bottom, right, 0.0, text);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_textboxrot
PURPOSE display some text at a jaunty angle on the PDF page specified

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_textbox (panda_pdf * output, panda_page * thisPage, int top, int left, int bottom, int right, double angle, char *text);
SYNOPSIS END

DESCRIPTION This function call creates a textbox on the specified page, and then displays the specified text within that page. The text is displayed at the spefied angle. The current font mode and style et cetera will be used. Sometime in the near future, line wrapping will be used...

RETURNS Nothing

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_page *page;

panda_init();

document = panda_open("filename.pdf", "w");
page = panda_newpage (document, panda_pagesize_a4);
panda_textboxrot (demo, currPage, 20 + (lineDepth * 20), 200, 40 + (lineDepth * 20), 400, 33.0, "Demonstration of a text mode");
EXAMPLE END
SEEALSO panda_createfont, panda_setfont, panda_panda_setfontsize, panda_getfontobj, panda_setfontmode, panda_setcharacterspacing, panda_setwordspacing, panda_sethorizontalscaling, panda_setleading, panda_textbox
DOCBOOK END
******************************************************************************/

void
panda_textboxrot (panda_pdf * output, panda_page * thisPage, int top, int left,
	       int bottom, int right, double angle, char *text)
{
  // Add a box with some text in it into the PDF page
  panda_object *textobj;
  char *currentToken, *strtokVictim = NULL, *delim, *tempPtr, *dictkey;
  int internalTop, internalLeft, displayedFirstPart = panda_false;
  panda_object *fontObj;

  /***************************************************************************
     Some text handling
  ***************************************************************************/

  // Make an object to store the text
  textobj = thisPage->contents;

  // Is there a font setup? Does this work with changing the font?
  if (output->currentFont == NULL){
    panda_setfont (output,
		   tempPtr = panda_createfont (output, "Helvetica", 1,
					       "MacRomanEncoding"));
    panda_xfree(tempPtr);
  }

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
      printf ("Searching for the font object relating to \"%s\"\n",
	      output->currentFont);
#endif
      if ((fontObj = panda_getfontobj (output, output->currentFont)) == NULL)
	panda_error (panda_true, "Could not find the font requested.");

#if defined DEBUG
      printf("Found 0x%08x (num %d gen %d)\n", fontObj, fontObj->number,
	     fontObj->generation);
#endif

      dictkey = panda_xsnprintf("Resources/Font/%s", output->currentFont);
      panda_adddictitem (output, thisPage->obj, dictkey,
			 panda_objectvalue, fontObj);
      panda_xfree(dictkey);
    }

  return;
  // Is there a text size setup?
  if (output->currentFontSize == -1)
    panda_setfontsize (output, 16);

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
  if (textobj->textmode == panda_false)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "\nBT\n");
      textobj->textmode = panda_true;

#if defined DEBUG
      printf ("Textbox: Entered textmode for object %d\n", textobj->number);
#endif
    }

  // This is a little dodgy at the moment because I do not understand the
  // PS matrix environment well enough to be writing this sort of code. I am
  // going to have to have a look into this a little more...
  textobj->layoutstream =
    panda_streamprintf (textobj->layoutstream, 
			"%.2f %.2f %.2f %.2f %d %d Tm\n",
			cos (angle * panda_pi / 180.0),
			sin (angle * panda_pi / 180.0),
			-sin (angle * panda_pi / 180.0),
			cos (angle * panda_pi / 180.0),
			internalLeft, internalTop);

  // There are now a whole bunch of options that may or may not need to be set
  if (output->currentFontMode != -1)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "%d Tr\n",
			    output->currentFontMode);
    }

  if (output->currentCharacterSpacing != 0.0)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "%.2f Tc\n",
			    output->currentCharacterSpacing);
    }

  if (output->currentWordSpacing != 0.0)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "%.2f Tw\n",
			    output->currentWordSpacing);
    }

  if (output->currentHorizontalScaling != 1.0)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "%.0f Tz\n",
			    output->currentHorizontalScaling * 100);
    }

  if (output->currentLeading != 0)
    {
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "%.2f TL\n",
			    output->currentLeading);
    }

  // If the font that we are using on the page is not the font that is
  // currently set, then the font has changed and we will need to define the
  // font here
  if ((textobj->currentSetFont == NULL) ||
      (strcmp (output->currentFont, textobj->currentSetFont) != 0))
    {
      // Set the font that we want to use
      textobj->layoutstream =
	panda_streamprintf (textobj->layoutstream, "/%s %d Tf\n",
			    output->currentFont, output->currentFontSize);

      // If we previously had one set, then get rid of it
      panda_xfree(textobj->currentSetFont);

      // Make space for the new name
#if defined DEBUG
      printf("Defining the current set font for object %d\n", textobj->number);
#endif

      textobj->currentSetFont =
	(char *) panda_xmalloc (sizeof (char) *
				(strlen (output->currentFont) + 1));

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
  strtokVictim = (char *) panda_xmalloc (sizeof (char) * (strlen (text) + 1));
  strcpy (strtokVictim, text);

  // Build the delimiter string
  delim = panda_xsnprintf ("\n%c%c%c", 4, 5, 6);

  currentToken = strtok (strtokVictim, delim);

  while (currentToken != NULL)
    {
      // If we haven't displayed that first part that would otherwise be missed
      // do so now
      if (displayedFirstPart == panda_false)
	{
	  textobj->layoutstream =
	    panda_streamprintf (textobj->layoutstream, "(%s) '\n",
				strtokVictim);
	  displayedFirstPart = panda_true;
	}

      switch (text[currentToken - strtokVictim - 1])
	{
	case '\n':
	  textobj->layoutstream =
	    panda_streamprintf (textobj->layoutstream, "(%s) '\n",
				currentToken);
	  break;

	case 4:
	  textobj->layoutstream = panda_streamprintf (textobj->layoutstream,
						      "%c Ts (%s) Tj\n",
						      currentToken[0],
						      currentToken + 1);
	  break;

	case 5:
	  textobj->layoutstream = panda_streamprintf (textobj->layoutstream,
						      "-%c Ts (%s) Tj\n",
						      currentToken[0],
						      currentToken + 1);
	  break;

	case 6:
	  textobj->layoutstream = panda_streamprintf (textobj->layoutstream,
						      "0 Ts (%s) Tj\n",
						      currentToken);
	  break;
	}

      currentToken = strtok (NULL, delim);
    }

  // Free temp data
  panda_xfree (strtokVictim);
}
