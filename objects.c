/******************************************************************************
  Objects.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda is based on the concept of panda_objects. This file contains all of
    methods needed to maintain the panda_objects that we have.
******************************************************************************/

#include <panda/functions.h>
#include <panda/constants.h>

/******************************************************************************
  Create an panda_object with panda_error checking.
******************************************************************************/

panda_object *
panda_newpanda_object (panda_pdf * doc, int type)
{
  // Do all the magic needed to create an panda_object
  panda_object *created;

  // Get some memory
  created = (panda_object *) panda_xmalloc(sizeof (panda_object));

  // We have no panda_children at the moment
  created->panda_children = (panda_child *) panda_xmalloc(sizeof (panda_child));

  ((panda_child *) created->panda_children)->next = NULL;
  (panda_child *) created->cachedLastChild = NULL;

  // Initialise the panda_dictionary
  created->dict = (panda_dictionary *) panda_xmalloc(sizeof (panda_dictionary));
  created->dict->next = NULL;

  // By default this panda_object is not a panda_pages panda_object
  created->isPages = gFalse;

  if (type == gPlaceholder)
    {
      // This is a placeholder panda_object, therefore it's number is -1
      created->number = -1;

      return created;
    }

  // Initialise the panda_object number
  created->number = doc->nextObjectNumber++;

#if defined DEBUG
  printf ("Created panda_object %d\n", created->number);
#endif

  // We don't have any streams at the moment
  created->binarystream = NULL;
  created->layoutstream = NULL;

  // There is no font defined
  created->currentSetFont = NULL;

  // We are not in text mode within the layout stream at the start
  created->textmode = gFalse;

  // New panda_objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new panda_object to the end of the linked list that we use to append
  // the panda_xref table onto the end of the PDF
  doc->panda_xrefTail->this = created;

  // Make space for the next one
  doc->panda_xrefTail->next = panda_xmalloc(sizeof (panda_xref));
  doc->panda_xrefTail->next->next = NULL;
  doc->panda_xrefTail = doc->panda_xrefTail->next;

  // Return
  return created;
}

panda_dictionary *
panda_adddictitem (panda_dictionary * input, char *name, int valueType, ...)
{
  // Add an item to the panda_dictionary in the panda_object
  panda_dictionary *dictNow;
  va_list argPtr;
  char *value;
  panda_object *objValue;
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictValue, *prevDictValue;
  int overwriting = gFalse;

#if defined DEBUG
  printf ("Added panda_dictionary item %s to panda_object (type = %d)\n", name,
	  valueType);
  fflush (stdout);
#endif

  // Find the end of the panda_dictionary or something with this name already
  dictNow = input;
  while ((dictNow->next != NULL) && (strcmp (dictNow->name, name) != 0))
    {
      dictNow = dictNow->next;
    }

  // Make a new end to the panda_dictionary if needed
  if (dictNow->next == NULL)
    {
      dictNow->next = (panda_dictionary *) panda_xmalloc(sizeof(panda_dictionary));

      // Setup
      dictNow->next->next = NULL;
      dictNow->panda_objectarrayValue = NULL;
      dictNow->dictValue = NULL;
      dictNow->textValue = NULL;

#if defined DEBUG
      printf (" (This is a new panda_dictionary element)\n");
#endif
    }
  else
    {
#if defined DEBUG
      printf (" (Overwriting a panda_dictionary element)\n");
#endif
      overwriting = gTrue;
    }

  // Work with the last argument
  va_start (argPtr, valueType);

  // And add some content to this entry if needed
  if (overwriting == gFalse)
    {
      dictNow->name = (char *) panda_xmalloc((strlen (name) + 1) * sizeof (char));
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
	panda_error ("Overwriting some panda_dictionary types not yet supported.");
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
	(char *) panda_xmalloc((strlen (value) + 3) * sizeof (char));
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

      objValue = va_arg (argPtr, panda_object *);

      // We assume we need no more than 20 characters to store this. This
      // should be fine
      dictNow->textValue = (char *) panda_xmalloc(sizeof (char) * 20);

      sprintf (dictNow->textValue, "%d %d R", objValue->number,
	       objValue->generation);
      break;

    case gObjArrayValue:
      objValue = va_arg (argPtr, panda_object *);

      if (dictNow->panda_objectarrayValue == NULL)
	{
	  dictNow->panda_objectarrayValue = 
	    (panda_objectarray *) panda_xmalloc(sizeof (panda_objectarray));
	  dictNow->panda_objectarrayValue->next = NULL;
	}

      // Find the end of the panda_object array list
      currentObjectArray = dictNow->panda_objectarrayValue;
      while (currentObjectArray->next != NULL)
	currentObjectArray = currentObjectArray->next;

      // Make a new array entry
      currentObjectArray->next = 
	(panda_objectarray *) panda_xmalloc(sizeof (panda_objectarray *));

      // Append
      currentObjectArray->number = objValue->number;
      currentObjectArray->generation = objValue->generation;
      currentObjectArray->next->next = NULL;
      break;

    case gDictionaryValue:
      // This is a sub-panda_dictionary
      dictValue = va_arg (argPtr, panda_dictionary *);

      if (overwriting == gFalse)
	{
	  // This is a new panda_dictionary item, just copy the info across
	  dictNow->dictValue = dictValue;

#if defined DEBUG
	  printf ("Added a subpanda_dictionary in its full glory\n");
#endif
	}
      else
	{
	  // We are appending to a subpanda_dictionary item -- we need to go through all
	  // of the subpanda_dictionary items we just got handed and add them to the
	  // subpanda_dictionary that is already here
	  while (dictValue->next != NULL)
	    {
#if defined DEBUG
	      printf ("Adding a subpanda_dictionary element named %s to existing\n",
		      dictValue->name);
#endif

	      switch (dictValue->valueType)
		{
		case gTextValue:
		case gBracketedTextValue:
		case gLiteralTextValue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->textValue);
		  break;

		case gObjValue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       gLiteralTextValue, dictValue->textValue);
		  break;

		case gDictionaryValue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->dictValue);
		  break;

		case gIntValue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->intValue);
		  break;

		case gObjArrayValue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType,
			       dictValue->panda_objectarrayValue);
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

  // Return the panda_dictionary item we changed (used in the lexer)
  return dictNow;
}

void *
panda_getdictvalue (panda_dictionary * dictValue)
{
  // Return a panda_dictionary value
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
      return dictValue->panda_objectarrayValue;
      break;
    }

  return NULL;
}

panda_dictionary *
panda_getdict (panda_dictionary * head, char *name)
{
  while ((strcmp (head->name, name) != 0) && (head->next != NULL))
    head = head->next;

  if (head->next == NULL)
    return NULL;
  return head;
}

void
panda_freepanda_object (panda_pdf * output, panda_object * freeVictim)
{

#if defined DEBUG
  printf ("Cleaning up panda_object number %d\n", freeVictim->number);
#endif

  // We should skip placeholder panda_objects (I think)
  if (freeVictim->number != -1)
    {
      // Free the panda_object and all it's bits -- free of a NULL does nothing! But
      // not in dmalloc!!!
      if (freeVictim->layoutstream != NULL)
	free (freeVictim->layoutstream);
      if (freeVictim->binarystream != NULL)
	free (freeVictim->binarystream);
      if (freeVictim->currentSetFont != NULL)
	free (freeVictim->currentSetFont);

      panda_freepanda_dictionary (freeVictim->dict);
    }

  // free(freeVictim);
}

void
panda_freepanda_dictionary (panda_dictionary * freeDict)
{
  panda_dictionary *now, *prev;
  int endoftheline = gTrue;

  // Still need to free the panda_dictionary... This can be made more efficient
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
	    panda_freepanda_dictionary (now->dictValue);
	}
      else
	endoftheline = gFalse;

      free (now);

      if (prev != NULL)
	prev->next = NULL;
    }

  // And free that initial panda_dictionary element
  if (freeDict != NULL)
    {
      free (freeDict->name);
      if (freeDict->textValue != NULL)
	free (freeDict->textValue);
      if (freeDict->dictValue != NULL)
	panda_freepanda_dictionary (freeDict->dictValue);

      free (freeDict);
    }
}


void
panda_writepanda_object (panda_pdf * output, panda_object * dumpTarget)
{
  // Do all that is needed to dump a PDF panda_object to disk
  unsigned long count;

#if defined DEBUG
  printf ("Writing panda_object number %d\n", dumpTarget->number);
#endif

  // We don't dump place holder panda_objects (number = -1)
  if (dumpTarget->number != -1)
    {
      // Remember the byte offset that was the start of this panda_object -- this is
      // needed for the XREF later
      dumpTarget->byteOffset = output->byteOffset;

    /*************************************************************************
      Do we have a layoutstream? If we do, work out the length of the stream 
      and save that as a panda_dictionary element.

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
	  panda_adddictitem (dumpTarget->dict, "Length", gIntValue,
		       strlen (dumpTarget->layoutstream) - 1);
	}

      // We cannot have a layoutstream and a binary stream in the same panda_object
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_adddictitem (dumpTarget->dict, "Length", gIntValue,
		       dumpTarget->binarystreamLength);
	}

      // We are going to dump the named panda_object (and only the named panda_object) to 
      // disk
      panda_printf (output, "%d %d obj\n",
		 dumpTarget->number, dumpTarget->generation);

      panda_writepanda_dictionary (output, dumpTarget, dumpTarget->dict);

      // Do we have a layoutstream?
      if (dumpTarget->layoutstream != NULL)
	{
	  panda_print (output, "stream\n");
	  panda_printf (output, "%s", dumpTarget->layoutstream);
	  panda_print (output, "endstream\n");
	}

      // Do we have a binary stream? We cannot have both cause how would be 
      // differentiate?
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_print (output, "stream\n");

	  for (count = 0; count < dumpTarget->binarystreamLength; count++)
	    panda_putc (output, dumpTarget->binarystream[count]);

	  panda_print (output, "\nendstream\n");
	}

      panda_print (output, "endobj\n");
    }
}

void
panda_writepanda_dictionary (panda_pdf * output, panda_object * obj, panda_dictionary * incoming)
{
  // Recursively write the panda_dictionary out (including sub-dictionaries)
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictNow;
  int atBegining = gTrue;
  panda_child *currentKid;

#if defined DEBUG
  printf ("Starting to write a panda_dictionary\n");
#endif

  // The start of the panda_dictionary
  panda_print (output, "<<\n");

  // Enumerate the panda_dictionary elements
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
	  printf ("Writing a text value named %s into the panda_dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %s\n", dictNow->name, dictNow->textValue);

	  // If the type is type, then possibly output the Kids line for the panda_pages
	  // panda_object
	  if ((strcmp (dictNow->name, "Type") == 0)
	      && (obj->isPages == gTrue))
	    {
	      panda_print (output, "\t/Kids [");

	      // Do the dumping
	      currentKid = obj->panda_children;

	      while (currentKid->next != NULL)
		{
		  if (atBegining == gFalse)
		    panda_print (output, " ");
		  else
		    atBegining = gFalse;

		  panda_printf (output, "%d %d R",
			     currentKid->me->number,
			     currentKid->me->generation);

		  // Next
		  currentKid = currentKid->next;
		}

	      // End it all
	      panda_print (output, "]\n");
	    }
	  break;

	case gIntValue:
#if defined DEBUG
	  printf ("Writing a int value with name %s into the panda_dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %d\n", dictNow->name, dictNow->intValue);
	  break;

	case gObjArrayValue:
#if defined DEBUG
	  printf
	    ("Writing an panda_object array value with name %s into panda_dictionary\n",
	     dictNow->name);
#endif

	  // Start the array in the file
	  atBegining = gTrue;

	  panda_printf (output, "\t/%s [", dictNow->name);

	  // Go through the array list until the end
	  currentObjectArray = dictNow->panda_objectarrayValue;
	  while (currentObjectArray->next != NULL)
	    {
	      if (atBegining == gTrue)
		atBegining = gFalse;
	      else
		panda_print (output, " ");

	      panda_printf (output, "%d %d R",
			 currentObjectArray->number,
			 currentObjectArray->generation);

	      currentObjectArray = currentObjectArray->next;
	    }

	  // Finish the array
	  panda_print (output, "]\n");
	  break;

	case gDictionaryValue:
	  // These are handled recursively
	  if (dictNow->dictValue == NULL)
	    panda_error ("Subpanda_dictionary value erroneously NULL.");

#if defined DEBUG
	  printf ("Output the subpanda_dictionary starting with the name %s\n",
		  dictNow->dictValue->name);
#endif

	  panda_printf (output, "\t/%s ", dictNow->name);

	  panda_writepanda_dictionary (output, output->dummyObj, dictNow->dictValue);
	  break;

	default:
	  panda_error ("Unknown panda_dictionary type");
	}

      dictNow = dictNow->next;
    }

  panda_print (output, ">>\n");
}

void
panda_addpanda_child (panda_object * parentObj, panda_object * panda_childObj)
{
  panda_child *currentChild = parentObj->panda_children;

  // Find the end of the list of panda_children
  if (parentObj->cachedLastChild != NULL)
    currentChild = parentObj->cachedLastChild;

  while (currentChild->next != NULL)
    currentChild = currentChild->next;

  // Make a new end
  currentChild->next = (panda_child *) panda_xmalloc(sizeof (panda_child));
  currentChild->next->next = NULL;

  // Make me be the panda_child panda_object
  currentChild->me = panda_childObj;

  // Cache it
  parentObj->cachedLastChild = currentChild;
}

void
panda_traversepanda_objects (panda_pdf * output, panda_object * dumpTarget, int direction,
		 traverseFunct function)
{
  // Write out an panda_object and all of it's panda_children. This may be done with
  // recursive calls and panda_writepanda_object()
  panda_child *currentChild;

  // No panda_children
  if (((panda_child *) dumpTarget->panda_children)->next == NULL)
    {
      function (output, dumpTarget);
      return;
    }

  // Otherwise, for me and then for all panda_children
  if (direction == gDown)
    function (output, dumpTarget);

  currentChild = dumpTarget->panda_children;
  while (currentChild->next != NULL)
    {
      panda_traversepanda_objects (output, currentChild->me, direction, function);
      currentChild = currentChild->next;
    }

  if (direction == gUp)
    function (output, dumpTarget);
}
