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

#include <panda/functions.h>
#include <panda/constants.h>

/******************************************************************************
  Create an object with error checking.
******************************************************************************/

object *
newobject (pdf * doc, int type)
{
  // Do all the magic needed to create an object
  object *created;

  // Get some memory
  created = (object *) xmalloc(sizeof (object));

  // We have no children at the moment
  created->children = (child *) xmalloc(sizeof (child));

  ((child *) created->children)->next = NULL;
  (child *) created->cachedLastChild = NULL;

  // Initialise the dictionary
  created->dict = (dictionary *) xmalloc(sizeof (dictionary));
  created->dict->next = NULL;

  // By default this object is not a pages object
  created->isPages = gFalse;

  if (type == gPlaceholder)
    {
      // This is a placeholder object, therefore it's number is -1
      created->number = -1;

      return created;
    }

  // Initialise the object number
  created->number = doc->nextObjectNumber++;

#if defined DEBUG
  printf ("Created object %d\n", created->number);
#endif

  // We don't have any streams at the moment
  created->binarystream = NULL;
  created->layoutstream = NULL;

  // There is no font defined
  created->currentSetFont = NULL;

  // We are not in text mode within the layout stream at the start
  created->textmode = gFalse;

  // New objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new object to the end of the linked list that we use to append
  // the xref table onto the end of the PDF
  doc->xrefTail->this = created;

  // Make space for the next one
  doc->xrefTail->next = xmalloc(sizeof (xref));
  doc->xrefTail->next->next = NULL;
  doc->xrefTail = doc->xrefTail->next;

  // Return
  return created;
}

dictionary *
adddictitem (dictionary * input, char *name, int valueType, ...)
{
  // Add an item to the dictionary in the object
  dictionary *dictNow;
  va_list argPtr;
  char *value;
  object *objValue;
  objectArray *currentObjectArray;
  dictionary *dictValue, *prevDictValue;
  int overwriting = gFalse;

#if defined DEBUG
  printf ("Added dictionary item %s to object (type = %d)\n", name,
	  valueType);
  fflush (stdout);
#endif

  // Find the end of the dictionary or something with this name already
  dictNow = input;
  while ((dictNow->next != NULL) && (strcmp (dictNow->name, name) != 0))
    {
      dictNow = dictNow->next;
    }

  // Make a new end to the dictionary if needed
  if (dictNow->next == NULL)
    {
      dictNow->next = (dictionary *) xmalloc(sizeof(dictionary));

      // Setup
      dictNow->next->next = NULL;
      dictNow->objectArrayValue = NULL;
      dictNow->dictValue = NULL;
      dictNow->textValue = NULL;

#if defined DEBUG
      printf (" (This is a new dictionary element)\n");
#endif
    }
  else
    {
#if defined DEBUG
      printf (" (Overwriting a dictionary element)\n");
#endif
      overwriting = gTrue;
    }

  // Work with the last argument
  va_start (argPtr, valueType);

  // And add some content to this entry if needed
  if (overwriting == gFalse)
    {
      dictNow->name = (char *) xmalloc((strlen (name) + 1) * sizeof (char));
      strcpy (dictNow->name, name);

      // Record the type
      dictNow->valueType = valueType;
    }
  else
    switch (valueType)
      {
      case gObjArrayValue:
      case gDictionaryValue:
      case gTextValue:
      case gLiteralTextValue:
      case gBracketedTextValue:
      case gIntValue:
      case gObjValue:
	break;

      default:
	error ("Overwriting some dictionary types not yet supported.");
      }

  switch (valueType)
    {
    case gTextValue:
    case gLiteralTextValue:
    case gBracketedTextValue:
      // Are we overwriting?
      if ((overwriting == gTrue) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      // Get the value
      value = va_arg (argPtr, char *);
      dictNow->textValue = 
	(char *) xmalloc((strlen (value) + 3) * sizeof (char));
      dictNow->textValue[0] = '\0';

      // Some stuff for different types
      if (valueType == gTextValue)
	strcat (dictNow->textValue, "/");
      if (valueType == gBracketedTextValue)
	strcat (dictNow->textValue, "(");

      // The string
      strcat (dictNow->textValue, value);

      // Some more stuff for different types
      if (valueType == gBracketedTextValue)
	strcat (dictNow->textValue, ")");
      break;

    case gIntValue:
      dictNow->intValue = va_arg (argPtr, int);
      break;

    case gObjValue:
      // Are we overwriting?
      if ((overwriting == gTrue) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      objValue = va_arg (argPtr, object *);

      // We assume we need no more than 20 characters to store this. This
      // should be fine
      dictNow->textValue = (char *) xmalloc(sizeof (char) * 20);

      sprintf (dictNow->textValue, "%d %d R", objValue->number,
	       objValue->generation);
      break;

    case gObjArrayValue:
      objValue = va_arg (argPtr, object *);

      if (dictNow->objectArrayValue == NULL)
	{
	  dictNow->objectArrayValue = 
	    (objectArray *) xmalloc(sizeof (objectArray));
	  dictNow->objectArrayValue->next = NULL;
	}

      // Find the end of the object array list
      currentObjectArray = dictNow->objectArrayValue;
      while (currentObjectArray->next != NULL)
	currentObjectArray = currentObjectArray->next;

      // Make a new array entry
      currentObjectArray->next = 
	(objectArray *) xmalloc(sizeof (objectArray *));

      // Append
      currentObjectArray->number = objValue->number;
      currentObjectArray->generation = objValue->generation;
      currentObjectArray->next->next = NULL;
      break;

    case gDictionaryValue:
      // This is a sub-dictionary
      dictValue = va_arg (argPtr, dictionary *);

      if (overwriting == gFalse)
	{
	  // This is a new dictionary item, just copy the info across
	  dictNow->dictValue = dictValue;

#if defined DEBUG
	  printf ("Added a subdictionary in its full glory\n");
#endif
	}
      else
	{
	  // We are appending to a subdictionary item -- we need to go through all
	  // of the subdictionary items we just got handed and add them to the
	  // subdictionary that is already here
	  while (dictValue->next != NULL)
	    {
#if defined DEBUG
	      printf ("Adding a subdictionary element named %s to existing\n",
		      dictValue->name);
#endif

	      switch (dictValue->valueType)
		{
		case gTextValue:
		case gBracketedTextValue:
		case gLiteralTextValue:
		  adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->textValue);
		  break;

		case gObjValue:
		  adddictitem (dictNow->dictValue, dictValue->name,
			       gLiteralTextValue, dictValue->textValue);
		  break;

		case gDictionaryValue:
		  adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->dictValue);
		  break;

		case gIntValue:
		  adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->intValue);
		  break;

		case gObjArrayValue:
		  adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType,
			       dictValue->objectArrayValue);
		  break;
		}

	      prevDictValue = dictValue;
	      dictValue = dictValue->next;
	    }
	}
      break;
    }

  // Stop dealing with arguments
  va_end (argPtr);

  // Return the dictionary item we changed (used in the lexer)
  return dictNow;
}

void *
getdictvalue (dictionary * dictValue)
{
  // Return a dictionary value
  switch (dictValue->valueType)
    {
    case gDictionaryValue:
      return dictValue->dictValue;
      break;

      // This line is a little scary -- we are going to make the int look like
      // a pointer for just a little while
    case gIntValue:
      return dictValue->intValue;
      break;

    case gTextValue:
    case gLiteralTextValue:
    case gObjValue:
      return dictValue->textValue;
      break;

    case gObjArrayValue:
      return dictValue->objectArrayValue;
      break;
    }

  return NULL;
}

dictionary *
getdict (dictionary * head, char *name)
{
  while ((strcmp (head->name, name) != 0) && (head->next != NULL))
    head = head->next;

  if (head->next == NULL)
    return NULL;
  return head;
}

void
freeObject (pdf * output, object * freeVictim)
{

#if defined DEBUG
  printf ("Cleaning up object number %d\n", freeVictim->number);
#endif

  // We should skip placeholder objects (I think)
  if (freeVictim->number != -1)
    {
      // Free the object and all it's bits -- free of a NULL does nothing! But
      // not in dmalloc!!!
      if (freeVictim->layoutstream != NULL)
	free (freeVictim->layoutstream);
      if (freeVictim->binarystream != NULL)
	free (freeVictim->binarystream);
      if (freeVictim->currentSetFont != NULL)
	free (freeVictim->currentSetFont);

      freeDictionary (freeVictim->dict);
    }

  // free(freeVictim);
}

void
freeDictionary (dictionary * freeDict)
{
  dictionary *now, *prev;
  int endoftheline = gTrue;

  // Still need to free the dictionary... This can be made more efficient
  while (freeDict->next != NULL)
    {
      now = freeDict;
      prev = NULL;

      while (now->next != NULL)
	{
	  prev = now;
	  now = now->next;
	}

      if (endoftheline == gFalse)
	{
	  free (now->name);
	  if (now->textValue != NULL)
	    free (now->textValue);
	  if (now->dictValue != NULL)
	    freeDictionary (now->dictValue);
	}
      else
	endoftheline = gFalse;

      free (now);

      if (prev != NULL)
	prev->next = NULL;
    }

  // And free that initial dictionary element
  if (freeDict != NULL)
    {
      free (freeDict->name);
      if (freeDict->textValue != NULL)
	free (freeDict->textValue);
      if (freeDict->dictValue != NULL)
	freeDictionary (freeDict->dictValue);

      free (freeDict);
    }
}


void
writeObject (pdf * output, object * dumpTarget)
{
  // Do all that is needed to dump a PDF object to disk
  unsigned long count;

#if defined DEBUG
  printf ("Writing object number %d\n", dumpTarget->number);
#endif

  // We don't dump place holder objects (number = -1)
  if (dumpTarget->number != -1)
    {
      // Remember the byte offset that was the start of this object -- this is
      // needed for the XREF later
      dumpTarget->byteOffset = output->byteOffset;

    /*************************************************************************
      Do we have a layoutstream? If we do, work out the length of the stream 
      and save that as a dictionary element.

      We also handle binarystreams here.

    *************************************************************************/

#if defined DEBUG
      printf ("Layoutstream is %s\n",
	      (dumpTarget->layoutstream == NULL) ? "NULL" : "not NULL");
      printf ("Binarystream is %s\n",
	      (dumpTarget->binarystream == NULL) ? "NULL" : "not NULL");
#endif

      if (dumpTarget->layoutstream != NULL)
	{
	  adddictitem (dumpTarget->dict, "Length", gIntValue,
		       strlen (dumpTarget->layoutstream) - 1);
	}

      // We cannot have a layoutstream and a binary stream in the same object
      else if (dumpTarget->binarystream != NULL)
	{
	  adddictitem (dumpTarget->dict, "Length", gIntValue,
		       dumpTarget->binarystreamLength);
	}

      // We are going to dump the named object (and only the named object) to 
      // disk
      pdfprintf (output, "%d %d obj\n",
		 dumpTarget->number, dumpTarget->generation);

      writeDictionary (output, dumpTarget, dumpTarget->dict);

      // Do we have a layoutstream?
      if (dumpTarget->layoutstream != NULL)
	{
	  pdfprint (output, "stream\n");
	  pdfprintf (output, "%s", dumpTarget->layoutstream);
	  pdfprint (output, "endstream\n");
	}

      // Do we have a binary stream? We cannot have both cause how would be 
      // differentiate?
      else if (dumpTarget->binarystream != NULL)
	{
	  pdfprint (output, "stream\n");

	  for (count = 0; count < dumpTarget->binarystreamLength; count++)
	    pdfputc (output, dumpTarget->binarystream[count]);

	  pdfprint (output, "\nendstream\n");
	}

      pdfprint (output, "endobj\n");
    }
}

void
writeDictionary (pdf * output, object * obj, dictionary * incoming)
{
  // Recursively write the dictionary out (including sub-dictionaries)
  objectArray *currentObjectArray;
  dictionary *dictNow;
  int atBegining = gTrue;
  child *currentKid;

#if defined DEBUG
  printf ("Starting to write a dictionary\n");
#endif

  // The start of the dictionary
  pdfprint (output, "<<\n");

  // Enumerate the dictionary elements
  dictNow = incoming;

  while (dictNow->next != NULL)
    {
      switch (dictNow->valueType)
	{
	case gTextValue:
	case gObjValue:
	case gLiteralTextValue:
	case gBracketedTextValue:
#if defined DEBUG
	  printf ("Writing a text value named %s into the dictionary\n",
		  dictNow->name);
#endif

	  pdfprintf (output, "\t/%s %s\n", dictNow->name, dictNow->textValue);

	  // If the type is type, then possibly output the Kids line for the pages
	  // object
	  if ((strcmp (dictNow->name, "Type") == 0)
	      && (obj->isPages == gTrue))
	    {
	      pdfprint (output, "\t/Kids [");

	      // Do the dumping
	      currentKid = obj->children;

	      while (currentKid->next != NULL)
		{
		  if (atBegining == gFalse)
		    pdfprint (output, " ");
		  else
		    atBegining = gFalse;

		  pdfprintf (output, "%d %d R",
			     currentKid->me->number,
			     currentKid->me->generation);

		  // Next
		  currentKid = currentKid->next;
		}

	      // End it all
	      pdfprint (output, "]\n");
	    }
	  break;

	case gIntValue:
#if defined DEBUG
	  printf ("Writing a int value with name %s into the dictionary\n",
		  dictNow->name);
#endif

	  pdfprintf (output, "\t/%s %d\n", dictNow->name, dictNow->intValue);
	  break;

	case gObjArrayValue:
#if defined DEBUG
	  printf
	    ("Writing an object array value with name %s into dictionary\n",
	     dictNow->name);
#endif

	  // Start the array in the file
	  atBegining = gTrue;

	  pdfprintf (output, "\t/%s [", dictNow->name);

	  // Go through the array list until the end
	  currentObjectArray = dictNow->objectArrayValue;
	  while (currentObjectArray->next != NULL)
	    {
	      if (atBegining == gTrue)
		atBegining = gFalse;
	      else
		pdfprint (output, " ");

	      pdfprintf (output, "%d %d R",
			 currentObjectArray->number,
			 currentObjectArray->generation);

	      currentObjectArray = currentObjectArray->next;
	    }

	  // Finish the array
	  pdfprint (output, "]\n");
	  break;

	case gDictionaryValue:
	  // These are handled recursively
	  if (dictNow->dictValue == NULL)
	    error ("Subdictionary value erroneously NULL.");

#if defined DEBUG
	  printf ("Output the subdictionary starting with the name %s\n",
		  dictNow->dictValue->name);
#endif

	  pdfprintf (output, "\t/%s ", dictNow->name);

	  writeDictionary (output, output->dummyObj, dictNow->dictValue);
	  break;

	default:
	  error ("Unknown dictionary type");
	}

      dictNow = dictNow->next;
    }

  pdfprint (output, ">>\n");
}

void
addchild (object * parentObj, object * childObj)
{
  child *currentChild = parentObj->children;

  // Find the end of the list of children
  if (parentObj->cachedLastChild != NULL)
    currentChild = parentObj->cachedLastChild;

  while (currentChild->next != NULL)
    currentChild = currentChild->next;

  // Make a new end
  currentChild->next = (child *) xmalloc(sizeof (child));
  currentChild->next->next = NULL;

  // Make me be the child object
  currentChild->me = childObj;

  // Cache it
  parentObj->cachedLastChild = currentChild;
}

void
traverseObjects (pdf * output, object * dumpTarget, int direction,
		 traverseFunct function)
{
  // Write out an object and all of it's children. This may be done with
  // recursive calls and writeObject()
  child *currentChild;

  // No children
  if (((child *) dumpTarget->children)->next == NULL)
    {
      function (output, dumpTarget);
      return;
    }

  // Otherwise, for me and then for all children
  if (direction == gDown)
    function (output, dumpTarget);

  currentChild = dumpTarget->children;
  while (currentChild->next != NULL)
    {
      traverseObjects (output, currentChild->me, direction, function);
      currentChild = currentChild->next;
    }

  if (direction == gUp)
    function (output, dumpTarget);
}
