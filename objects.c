/******************************************************************************
  Objects.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     Fixed bugs, added write object,                   17062000
                       addchild, byte offset code
                     Added XREF and object traversal code              18062000

  Purpose:
    Panda is based on the concept of objects. This file contains all of
    methods needed to maintain the objects that we have.
******************************************************************************/

#include "functions.h"
#include "constants.h"

/******************************************************************************
  Create an object with error checking.
******************************************************************************/

object *newobject(pdf *doc, int type){
  // Do all the magic needed to create an object
  object  *created;

  // Get some memory
  if((created = (object *) malloc(sizeof(object))) == NULL)
    error("Could not create an object (out of memory?)");

  // We have no children at the moment
  if((created->children = (child *) malloc(sizeof(child))) == NULL)
    error("Could not make a child for the new object.");

  ((child *) created->children)->next = NULL;
  (child *) created->cachedLastChild = NULL;

  // Initialise the dictionary
  if((created->dict = (dictionary *) malloc(sizeof(dictionary))) == NULL)
    error("Could not create the dictionary root for the new object.");

  created->dict->next = NULL;

  // By default this object is not a pages object
  created->isPages = gFalse;

  if(type == gPlaceholder){
    // This is a placeholder object, therefore it's number is -1
    created->number = -1;

    return created;
  }

  // Initialise the object number
  created->number = doc->nextObjectNumber++;

#if defined DEBUG
  printf("Created object %d\n", created->number);
#endif

  // We do not possess a textstream at the moment
  created->textstream = NULL;
  created->textstreamLength = 0;

  // And we don't have a binary stream either
  created->binarystream = NULL;
  created->binarystreamLength = 0;

  // Nor a xobjectstream
  created->xobjectstream = NULL;
  created->xobjectstreamLength = 0;

  // There is no font defined
  created->currentSetFont = NULL;

  // New objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new object to the end of the linked list that we use to append
  // the xref table onto the end of the PDF
  doc->xrefTail->this = created;  

  // Make space for the next one
  if((doc->xrefTail->next = malloc(sizeof(xref))) == NULL)
    error("Could not add xref to the list for new object.");
  doc->xrefTail->next->next = NULL;
  doc->xrefTail = doc->xrefTail->next;

  // Return
  return created;
}

void adddictitem(object *input, char *name, int valueType, ...){
  // Add an item to the dictionary in the object
  dictionary  *dictNow;
  va_list     argPtr;
  char        *value;
  object      *objValue;
  objectArray *currentObjectArray;
  dictionary  *dictValue;

#if defined DEBUG
  printf("Added dictionary item %s to object %d", name, input->number);
  fflush(stdout);
#endif

  // Find the end of the dictionary or something with this name already
  dictNow = input->dict;
  while((dictNow->next != NULL) && (strcmp(dictNow->name, name) != 0)){
    dictNow = dictNow->next;
    }

  // Make a new end to the dictionary if needed
  if(dictNow->next == NULL){
    if((dictNow->next = (dictionary *) malloc(sizeof(dictionary))) == NULL)
      error("Could not grow the dictionary.");

    // Setup
    dictNow->next->next = NULL;
    dictNow->objectArrayValue = NULL;
    dictNow->dictValue = NULL;
    }
  else{
#if defined DEBUG
    printf(" (Overwriting a dictionary element) ");
    fflush(stdout);
#endif
  }

  // Work with the last argument
  va_start(argPtr, valueType);

  // And add some content to this entry if needed
  if(dictNow->next->next == NULL){
    if((dictNow->name =
      (char *) malloc(strlen(name) * sizeof(char))) == NULL)
      error("Could not make space for the new tag name value.");
    strcpy(dictNow->name, name);

    // Record the type
    dictNow->valueType = valueType;
  }
  else if(valueType != gObjArrayValue)
    error("Overwriting non array objects not yet supported.");

  switch(valueType){
  case gTextValue:
  case gLiteralTextValue:
  case gBracketedTextValue:
    // Get the value
    value = va_arg(argPtr, char *);
    if((dictNow->textValue =
      (char *) malloc((strlen(value) + 2) * sizeof(char))) == NULL)
      error("Could not make space for the new dictionary text value.");
    dictNow->textValue[0] = '\0';

    // Some stuff for different types
    if(valueType == gTextValue) strcat(dictNow->textValue, "/");
    if(valueType == gBracketedTextValue) strcat(dictNow->textValue, "(");

    // The string
    strcat(dictNow->textValue, value);

    // Some more stuff for different types
    if(valueType == gBracketedTextValue) strcat(dictNow->textValue, ")");
    break;

  case gIntValue:
    dictNow->textValue = NULL;
    dictNow->intValue = va_arg(argPtr, int);
    break;

  case gObjValue:
    objValue = va_arg(argPtr, object *);

    // We assume we need no more than 20 characters to store this. This
    // should be fine
    if((dictNow->textValue =
      (char *) malloc(sizeof(char) * 20)) == NULL)
      error("Could not make space for the new dictionary object value.");
    sprintf(dictNow->textValue, "%d %d R", objValue->number,
      objValue->generation);
    break;

  case gObjArrayValue:
    objValue = va_arg(argPtr, object *);

    if(dictNow->objectArrayValue == NULL){
      if((dictNow->objectArrayValue =
        (objectArray *) malloc(sizeof(objectArray))) == NULL)
        error("Could not create the object array root for the new dictionary.");
      dictNow->objectArrayValue->next = NULL;
      }

    // Find the end of the object array list
    currentObjectArray = dictNow->objectArrayValue;
    while(currentObjectArray->next != NULL)
      currentObjectArray = currentObjectArray->next;

    // Make a new array entry
    if((currentObjectArray->next =
      (objectArray *) malloc(sizeof(objectArray *))) == NULL)
      error("Could not append to the object array.");

    // Append
    currentObjectArray->number = objValue->number;
    currentObjectArray->generation = objValue->generation;
    currentObjectArray->next->next = NULL;
    break;

  case gDictionaryValue:
    // This is a sub-dictionary
    dictValue = va_arg(argPtr, dictionary *);

    // And now do the adding
    dictNow->dictValue = dictValue;
    break;
  }

  // Stop dealing with arguments
  va_end(argPtr);

#if defined DEBUG
  printf(" -> %d\n", input->number);
#endif
}

void *getdictvalue(dictionary *dictValue){
  // Return a dictionary value
  switch(dictValue->valueType){
  case gDictionaryValue:  return dictValue->dictValue;    break;

  // This line is a little scary -- we are going to make the int look like
  // a pointer for just a little while
  case gIntValue:       return dictValue->intValue;         break;

  case gTextValue:
  case gLiteralTextValue:
  case gObjValue:
                        return dictValue->textValue;        break;

  case gObjArrayValue:  return dictValue->objectArrayValue; break;
  }

  return NULL;
}

void writeObject(pdf *output, object *dumpTarget){
  // Do all that is needed to dump a PDF object to disk
  unsigned long  count;

  // We don't dump place holder objects (number = -1)
  if(dumpTarget->number != -1){
    // Remember the byte offset that was the start of this object -- this is
    // needed for the XREF later
    dumpTarget->byteOffset = output->byteOffset;

    /*************************************************************************
      Do we have a textstream? If we do, work out the length of the stream and
      save that as a dictionary element -- this needs to include all of the
      different stream types eventually...

      The +6 is because of the BT and ET that are added at this stage...

      There is also the option that we have commands that go after the
      stream from the xobjectsstream, which we need to check for as well.

      We also handle binarystreams here.

    *************************************************************************/

    if(dumpTarget->textstreamLength > 0){
      // Do we also have an xobjectstream?
      adddictitem(dumpTarget, "Length", gIntValue, 
        dumpTarget->textstreamLength + 6 + dumpTarget->xobjectstream);
    }

    // We cannot have a textstream and a binary stream in the same object
    else if(dumpTarget->binarystreamLength > 0){
      adddictitem(dumpTarget, "Length", gIntValue,
	dumpTarget->binarystreamLength);
    }

    // We might also only have an xobjectstream here
    if(dumpTarget->xobjectstreamLength > 0){
      adddictitem(dumpTarget, "Length", gIntValue, 
	dumpTarget->xobjectstreamLength);
    }
    
    // We are going to dump the named object (and only the named object) to 
    // disk
    pdfprintf(output, "%d %d obj\n", 
      dumpTarget->number,
      dumpTarget->generation);

    writeDictionary(output, dumpTarget, dumpTarget->dict);

    // Do we have a textstream?
    if((dumpTarget->textstreamLength > 0) && (dumpTarget->textstream != NULL)){
      pdfprint(output, "stream\nBT\n");
      pdfprintf(output, "%s", dumpTarget->textstream);
      pdfprint(output, "ET\n");

      // We might also have an xobjectstream
      if(dumpTarget->xobjectstreamLength > 0){
        pdfprintf(output, "%s", dumpTarget->xobjectstream);
      }

      pdfprint(output, "\nendstream\n");
    }

    // Do we have a binary stream? We cannot have both cause how would be 
    // differentiate?
    else if((dumpTarget->binarystreamLength > 0) &&
      (dumpTarget->binarystream != NULL)){
      pdfprint(output, "stream\n");

      for(count = 0; count < dumpTarget->binarystreamLength; count ++)
	pdfputc(output, dumpTarget->binarystream[count]);

      pdfprint(output, "\nendstream\n");
    }
    
    // We might also only have an xobject stream
    else if(dumpTarget->xobjectstream > 0){
      pdfprint(output, "stream\n");
      pdfprintf(output, "%s", dumpTarget->xobjectstream);
      pdfprint(output, "\nendstream\n");
    }

    pdfprint(output, "endobj\n");
  }
}

void writeDictionary(pdf *output, object *obj, dictionary *incoming){
  // Recursively write the dictionary out (including sub-dictionaries)
  objectArray  *currentObjectArray;
  dictionary   *dictNow;
  int          atBegining = gTrue;
  child        *currentKid;

  // The start of the dictionary
  pdfprint(output, "<<\n");

  // Enumerate the dictionary elements
  dictNow = incoming;

  while(dictNow->next != NULL){
    switch(dictNow->valueType){
    case gTextValue:
    case gObjValue:
    case gLiteralTextValue:
    case gBracketedTextValue:
      pdfprintf(output, "\t/%s %s\n", dictNow->name, dictNow->textValue);

      // If the type is type, then possibly output the Kids line for the pages
      // object
      if((strcmp(dictNow->name, "Type") == 0) && (obj->isPages == gTrue)){
	pdfprint(output, "\t/Kids [");

	// Do the dumping
	currentKid = obj->children;

	while(currentKid->next != NULL){
	  if(atBegining == gFalse) pdfprint(output, " ");
	  else atBegining = gFalse;

	  pdfprintf(output, "%d %d R", 
	    currentKid->me->number,
	    currentKid->me->generation);

	  // Next
	  currentKid = currentKid->next;
	}
	
	// End it all
	pdfprint(output, "]\n");
      }
      break;

    case gIntValue:
      pdfprintf(output, "\t/%s %d\n", dictNow->name, dictNow->intValue);
      break;

    case gObjArrayValue:
      // Start the array in the file
      atBegining = gTrue;

      pdfprintf(output, "\t/%s [", dictNow->name);

      // Go through the array list until the end
      currentObjectArray = dictNow->objectArrayValue;
      while(currentObjectArray->next != NULL){
        if(atBegining == gTrue) atBegining = gFalse;
        else pdfprint(output, " ");

        pdfprintf(output, "%d %d R",
          currentObjectArray->number,
	  currentObjectArray->generation);

        currentObjectArray = currentObjectArray->next;
      }

      // Finish the array
      pdfprint(output, "]\n");
      break;

    case gDictionaryValue:
      // These are handled recursively
      pdfprintf(output, "\t/%s ", dictNow->name);

      writeDictionary(output, output->dummyObj, dictNow->dictValue);
      break;
    }

    dictNow = dictNow->next;
  }

  pdfprint(output, ">>\n");
}

void addchild(object *parentObj, object *childObj){
  child  *currentChild = parentObj->children;

  // Find the end of the list of children
  if(parentObj->cachedLastChild != NULL) 
    currentChild = parentObj->cachedLastChild;

  while(currentChild->next != NULL) currentChild = currentChild->next;

  // Make a new end
  if((currentChild->next = (child *) malloc(sizeof(child))) == NULL)
    error("Could not add the child to the end of the list.");
  currentChild->next->next = NULL;

  // Make me be the child object
  currentChild->me = childObj;

  // Cache it
  parentObj->cachedLastChild = currentChild;
}

void traverseObjects(pdf *output, object *dumpTarget, traverseFunct function){
  // Write out an object and all of it's children. This may be done with
  // recursive calls and writeObject()
  child  *currentChild;

  // No children
  if(((child *) dumpTarget->children)->next == NULL){
    function(output, dumpTarget);
    return;
  }

  // Otherwise, for me and then for all children
  function(output, dumpTarget);

  currentChild = dumpTarget->children;
  while(currentChild->next != NULL){
    traverseObjects(output, currentChild->me, function);
    currentChild = currentChild->next;
  }
}

void appendtextstream(object *target, char *data, unsigned long len){
  // We are going to append to the textstream that the object already has
  unsigned long  initial, count;
  
  initial = count = target->textstreamLength;

  // Increase the length of the textstream
  target->textstreamLength += len;

  // Make space for the new information
  if((target->textstream = realloc(target->textstream,
    sizeof(char) * target->textstreamLength)) == NULL)
    error("Could not append to an object's textstream.");

  // Append
  for(; count < target->textstreamLength; count++)
    target->textstream[count] = data[count - initial];
}

void appendxobjectstream(object *target, char *data, unsigned long len){
  // We are going to append to the xobjectstream that the object already has
  unsigned long  initial, count;
  
  initial = count = target->xobjectstreamLength;

  // Increase the length of the xobjectstream
  target->xobjectstreamLength += len;

  // Make space for the new information
  if((target->xobjectstream = realloc(target->xobjectstream,
    sizeof(char) * target->xobjectstreamLength)) == NULL)
    error("Could not append to an object's xobject stream.");

  // Append
  for(; count < target->xobjectstreamLength; count++)
    target->xobjectstream[count] = data[count - initial];
}
