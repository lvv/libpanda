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

void textbox(pdf *output, page *thisPage, int top, int left, int bottom,
       int right, char *text){
  // Add a box with some text in it into the PDF page
  object      *textobj, *tempObj;
  char        commandBuffer[1024];
  int         internalTop, internalLeft;
  object      *subdict, *subsubdict, *fontObj;

  /***************************************************************************
     Some text handling
  ***************************************************************************/

  // Make an object to store the text
  //  textobj = newobject(output, gNormal);
  //  addchild(thisPage->obj, textobj);
  //  adddictitem(thisPage->obj, "Contents", gObjValue, textobj);
  textobj = thisPage->contents;

  // Is there a font setup?
  if(output->currentFont == NULL){
    setfont(output,
      createfont(output, "Helvetica", 1, "MacRomanEncoding"));
  }

  // Is there a text size setup?
  if(output->currentFontSize == -1)
    setfontsize(output, 16);

  /***************************************************************************
    This font has to be referred to in the resources entry in the dictionary.

    The resources dictionary looks a lot like:
      /Resources <<
        /Font <<
          /...Name... ...Obj Reference...
          >>
        >>

    An example line is /MikalsFont 54 0 R
  ***************************************************************************/

  // Find the font object needed
  if((fontObj = getfontobj(output, output->currentFont)) == NULL)
    error("Could not find the font requested.");

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  subsubdict = newobject(output, gPlaceholder);
  adddictitem(subsubdict, output->currentFont, gObjValue, fontObj);
  
  subdict = newobject(output, gPlaceholder);
  adddictitem(subdict, "Font", gDictionaryValue, subsubdict->dict);

  // And put this into the PDF
  adddictitem(thisPage->obj, "Resources", gDictionaryValue, subdict->dict);
  
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

    There is no need for the Tr command if the text mode is 0
  ***************************************************************************/

  // Build the command buffer
  // This is a little dodgy at the moment because I do not understand the
  // PS matrix environment well enough to be writing this sort of code. I am
  // going to have to have a look into this a little more...
  sprintf(commandBuffer,
    "BT\n 1 0 0 1 %d %d Tm\n %d Tr\n/%s %d Tf\n (%s) '\nET",
    internalLeft, internalTop,
    output->currentFontMode,
    output->currentFont, output->currentFontSize,
    text);

  // This new object has a stream with the text in it
  appendstream(textobj, commandBuffer, strlen(commandBuffer));
}
