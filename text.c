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
  char        commandBuffer[1024], *currentToken, *strtokVictim = NULL;
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
  ***************************************************************************/

  // Build the command buffer
  // This is a little dodgy at the moment because I do not understand the
  // PS matrix environment well enough to be writing this sort of code. I am
  // going to have to have a look into this a little more...
  sprintf(commandBuffer,
    "BT\n 1 0 0 1 %d %d Tm\n",
    internalLeft, internalTop);

  // This new object has a stream with the text in it
  appendstream(textobj, commandBuffer, strlen(commandBuffer));

  // There are now a whole bunch of options that may or may not need to be set
  if(output->currentFontMode != 0){
    sprintf(commandBuffer, "%d Tr\n", output->currentFontMode);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  }

  if(output->currentCharacterSpacing != 0){
    sprintf(commandBuffer, "%.2f Tc\n", output->currentCharacterSpacing);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  }

  if(output->currentWordSpacing != 0){
    sprintf(commandBuffer, "%.2f Tw\n", output->currentWordSpacing);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  }

  if(output->currentHorizontalScaling != 1){
    sprintf(commandBuffer, "%.0f Tz\n",
      output->currentHorizontalScaling * 100);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  }

  if(output->currentLeading != 0){
    sprintf(commandBuffer, "%.2 TL\n", output->currentLeading);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  }

  // Set the font that we want to use
  sprintf(commandBuffer, "/%s %d Tf\n", 
    output->currentFont, output->currentFontSize);
  appendstream(textobj, commandBuffer, strlen(commandBuffer));

  /***************************************************************************
    PDFs are funny in that we have to specify where line breaks are to occur.
    We allow the programmer to specify line breaks by putting the characters
    '\n' into the string that we is having us put into the PDF. We therefore
    need to find this character sequence and handle it...
  ***************************************************************************/

  // Get the first token
  if((strtokVictim = (char *) malloc(sizeof(char) * strlen(text))) == NULL)
    error("Could not make space for temporary copy of textbox text.");
  strcpy(strtokVictim, text);

  currentToken = strtok(strtokVictim, "\n");

  while(currentToken != NULL){
    sprintf(commandBuffer, " (%s) '\n", currentToken);
    appendstream(textobj, commandBuffer, strlen(commandBuffer));
  
    currentToken = strtok(NULL, "\n");
  }

  /***************************************************************************
    Now finish the text off
  ***************************************************************************/
  appendstream(textobj, "ET", 2);
}
