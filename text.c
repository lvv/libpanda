/******************************************************************************
	text.c

	Change Control:													DDMMYYYY
		Michael Still		File created							22062000

	Purpose:
		Functions related to displaying text on a PDF page.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void textbox(pdf *output, page *thisPage, int top, int left, int bottom,
			 int right, char *text){
	// Add a box with some text in it into the PDF page
	object		*textobj, *tempObj;
	char		commandBuffer[1024];
	dictionary	*temp, *temp2;

	// Make an object to store the text
	textobj = newobject(output, gNormal);
	addchild(thisPage->obj, textobj);
	adddictitem(thisPage->obj->dict, "Contents", gObjValue, textobj);

	// Is there a font setup?
	if(output->currentFont == NULL){
		setfont(output,
			createfont(output, "Helvetica", 1, "MacRomanEncoding"));
	}

	/***************************************************************************
		This font has to be referred to in the resources entry in the dictionary
	***************************************************************************/

	// Build temp item and temp2 item
	if((temp = (dictionary *) malloc(sizeof(dictionary))) == NULL)
		error("Could not make a temp dictionary item.");
	temp->next = NULL;

	if((temp2 = (dictionary *) malloc(sizeof(dictionary))) == NULL)
		error("Could not make a second temp dictionary item.");
	temp2->next = NULL;
	
	// Temp2 is the name of the font and it's object
	if((tempObj = getfontobj(output, output->currentFont)) == NULL)
		error("Could not find the font requested.");
	adddictitem(temp2, output->currentFont, gObjValue, tempObj);

	// Temp is the word font and temp2
	adddictitem(temp, "Font", gDictionaryValue, temp2);

	// We need to add some entries into the resources dictionary for the object
	adddictitem(thisPage->obj->dict, "Resources", gDictionaryValue, temp);

	/***************************************************************************
		Back to inserting the text
	***************************************************************************/

	// Is there a text size setup?
	if(output->currentFontSize == -1)
		setfontsize(output, 16);

	// Build the command buffer
	sprintf(commandBuffer,
		"BT\n /%s %d Tf\n (%s) '\nET",
		output->currentFont, output->currentFontSize,
		text);

	// This new object has a stream with the text in it
	appendstream(textobj, commandBuffer, strlen(commandBuffer));
}
