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
  ((child *) created->cachedLastChild = NULL;

  // Initialise the dictionary
  if((created->dict = (dictionary *) malloc(sizeof(dictionary))) == NULL)
    error("Could not create the dictionary root for the new object.");

  created->dict->next = NULL;

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

  // There is no font defined
  created->currentSetFont = NULL;

  // New objects have a generation number of 0 by definition
  created->generation = 0;

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
    // Get the value
    value = va_arg(argPtr, char *);
    if((dictNow->textValue =
      (char *) malloc(strlen(value) * sizeof(char))) == NULL)
      error("Could not make space for the new dictionary text value.");
    dictNow->textValue[0] = '\0';
    if(valueType != gLiteralTextValue) strcat(dictNow->textValue, "/");
    strcat(dictNow->textValue, value);
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
  case gIntValue:      return dictValue->intValue;      break;

  case gTextValue:
  case gLiteralTextValue:
  case gObjValue:
              return dictValue->textValue;    break;

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

    *************************************************************************/

    // Textstream

    if((dumpTarget->textstreamLength > 0) && (dumpTarget->textstream != NULL)){
      adddictitem(dumpTarget, "Length", gIntValue, 
        dumpTarget->textstreamLength + 6);
    }
    
    // We are going to dump the named object (and only the named object) to 
    // disk
    pdfprintf(output, "%d %d obj\n", dumpTarget->number, 
      dumpTarget->generation);

    writeDictionary(output, dumpTarget->dict);

    // Do we have a textstream?
    if((dumpTarget->textstreamLength > 0) && (dumpTarget->textstream != NULL)){
      pdfprintf(output, "stream\nBT\n");

      // This is done because a valid stream could contain a \0, which
      // would confuse pdfprintf with a %s
      //for(count = 0; count < dumpTarget->textstreamLength; count ++)
      //  pdfputc(output, dumpTarget->textstream[count]);


      // We know that textstreams never contain a \0, so we can use pdfprintf
      pdfprintf(output, "%s\n", dumpTarget->textstream);
      pdfprintf(output, "ET\nendstream\n");
    }
    
    pdfprintf(output, "endobj\n");
  }
}

void writeDictionary(pdf *output, dictionary  *incoming){
  // Recursively write the dictionary out (including sub-dictionaries)
  objectArray  *currentObjectArray;
  dictionary   *dictNow;
  int          atBegining = gTrue;

  // The start of the dictionary
  pdfprintf(output, "<<\n");

  // Enumerate the dictionary elements
  dictNow = incoming;

  while(dictNow->next != NULL){
    switch(dictNow->valueType){
    case gTextValue:
    case gObjValue:
    case gLiteralTextValue:
      pdfprintf(output, "\t/%s %s\n", dictNow->name, dictNow->textValue);
      break;

    case gIntValue:
      pdfprintf(output, "\t/%s %d\n", dictNow->name, dictNow->intValue);
      break;

    case gObjArrayValue:
      // Start the array in the file
      pdfprintf(output, "\t/%s [", dictNow->name);

      // Go through the array list until the end
      currentObjectArray = dictNow->objectArrayValue;
      while(currentObjectArray->next != NULL){
        if(atBegining == gTrue) atBegining = gFalse;
        else pdfprintf(output, " ");

        pdfprintf(output, "%d %d R",
          currentObjectArray->number,
	  currentObjectArray->generation);

        currentObjectArray = currentObjectArray->next;
      }

      // Finish the array
      pdfprintf(output, "]\n");
      break;

    case gDictionaryValue:
      // These are handled recursively
      pdfprintf(output, "\t/%s ", dictNow->name);

      writeDictionary(output, dictNow->dictValue);
      break;
    }

    dictNow = dictNow->next;
  }

  pdfprintf(output, ">>\n");
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
  parentObj->cachedLastChild = childObj;
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
