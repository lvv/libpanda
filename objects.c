/******************************************************************************
  Objects.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda is based on the concept of objects. This file contains all of
    methods needed to maintain the objects that we have.
******************************************************************************/

#include <panda/functions.h>
#include <panda/constants.h>
#include <zlib.h>

/******************************************************************************
DOCBOOK START

FUNCTION panda_newobject
PURPOSE create a new object

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
panda_object *panda_newobject (panda_pdf *output, int type);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Create a new object within the PDF document output. The types of object are panda_object_normal and panda_object_placeholder -- the difference is that a placeholder object wont be written out when the PDF is dumped to disc. This is an internal function call and would only be need by API users if they are creating support for object types not currently supported by <command>Panda</command>. If this is the case, then these users are encouraged to submit their code changes back to mikal@stillhq.com for inclusion with the next release of <command>Panda</command>, and are reminded of their obligations under the GNU GPL.

RETURNS A pointer to the new object (a panda_object *)

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_object *obj;

panda_init();

document = panda_open("filename.pdf", "w");
obj = panda_newobject(document, panda_object_normal);
EXAMPLE END
SEEALSO panda_freeobject, panda_writeobject, panda_traverseobjects, panda_addchild
DOCBOOK END
******************************************************************************/

panda_object *
panda_newobject (panda_pdf * doc, int type)
{
  // Do all the magic needed to create an object
  panda_object *created;

  // Get some memory
  created = (panda_object *) panda_xmalloc (sizeof (panda_object));

  // We have no children at the moment
  created->children = (panda_child *) panda_xmalloc (sizeof (panda_child));

  ((panda_child *) created->children)->next = NULL;
  (panda_child *) created->cachedLastChild = NULL;

  // Initialise the dictionary
  created->dict =
    (panda_dictionary *) panda_xmalloc (sizeof (panda_dictionary));
  created->dict->next = NULL;

  // By default this object is not a pages object
  created->isPages = panda_false;
  created->isTemplate = panda_false;

  if (type == panda_placeholder)
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
  created->textmode = panda_false;

  // We are by default not a contents object
  created->isContents = panda_false;

  // New objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new object to the end of the linked list that we use to append
  // the xref table onto the end of the PDF
  doc->xrefTail->this = created;

  // Make space for the next one
  doc->xrefTail->next = panda_xmalloc (sizeof (panda_xref));
  doc->xrefTail->next->next = NULL;
  doc->xrefTail = doc->xrefTail->next;

  // Set the value of the local and cascaded proprties for this object to 
  // defaults (all off)
  memset (created->localproperties, panda_false, panda_object_property_max);
  memset (created->cascadeproperties, panda_false, panda_object_property_max);

  // The compression level is a little different
  created->localproperties[panda_object_property_compress_level] =
    created->cascadeproperties[panda_object_property_compress_level] =
    panda_default_compress_level;

  // Return
  return created;
}

panda_dictionary *
panda_adddictitem (panda_dictionary * input, char *name, int valueType, ...)
{
  // Add an item to the dictionary in the object
  panda_dictionary *dictNow;
  va_list argPtr;
  char *value;
  panda_object *objValue;
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictValue, *prevDictValue;
  int overwriting = panda_false;

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
      dictNow->next =
	(panda_dictionary *) panda_xmalloc (sizeof (panda_dictionary));

      // Setup
      dictNow->next->next = NULL;
      dictNow->objectarrayValue = NULL;
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
      overwriting = panda_true;
    }

  // Work with the last argument
  va_start (argPtr, valueType);

  // And add some content to this entry if needed
  if (overwriting == panda_false)
    {
      dictNow->name =
	(char *) panda_xmalloc ((strlen (name) + 1) * sizeof (char));
      strcpy (dictNow->name, name);

      // Record the type
      dictNow->valueType = valueType;
    }
  else
    switch (valueType)
      {
      case panda_objectarrayvalue:
      case panda_dictionaryvalue:
      case panda_textvalue:
      case panda_literaltextvalue:
      case panda_brackettedtextvalue:
      case panda_integervalue:
      case panda_objectvalue:
	break;

      default:
	panda_error ("Overwriting some dictionary types not yet supported.");
      }

  switch (valueType)
    {
    case panda_textvalue:
    case panda_literaltextvalue:
    case panda_brackettedtextvalue:
      // Are we overwriting?
      if ((overwriting == panda_true) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      // Get the value
      value = va_arg (argPtr, char *);
      dictNow->textValue =
	(char *) panda_xmalloc ((strlen (value) + 3) * sizeof (char));
      dictNow->textValue[0] = '\0';

      // Some stuff for different types
      if (valueType == panda_textvalue)
	strcat (dictNow->textValue, "/");
      if (valueType == panda_brackettedtextvalue)
	strcat (dictNow->textValue, "(");

      // The string
      strcat (dictNow->textValue, value);

      // Some more stuff for different types
      if (valueType == panda_brackettedtextvalue)
	strcat (dictNow->textValue, ")");
      break;

    case panda_integervalue:
      dictNow->intValue = va_arg (argPtr, int);
      break;

    case panda_objectvalue:
      // Are we overwriting?
      if ((overwriting == panda_true) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      objValue = va_arg (argPtr, panda_object *);

      // We assume we need no more than 20 characters to store this. This
      // should be fine
      dictNow->textValue = (char *) panda_xmalloc (sizeof (char) * 20);

      dictNow->textValue = panda_xsnprintf ("%d %d R", objValue->number,
					    objValue->generation);
      break;

    case panda_objectarrayvalue:
      objValue = va_arg (argPtr, panda_object *);

      if (dictNow->objectarrayValue == NULL)
	{
	  dictNow->objectarrayValue =
	    (panda_objectarray *) panda_xmalloc (sizeof (panda_objectarray));
	  dictNow->objectarrayValue->next = NULL;
	}

      // Find the end of the object array list
      currentObjectArray = dictNow->objectarrayValue;
      while (currentObjectArray->next != NULL)
	currentObjectArray = currentObjectArray->next;

      // Make a new array entry
      currentObjectArray->next =
	(panda_objectarray *) panda_xmalloc (sizeof (panda_objectarray *));

      // Append
      currentObjectArray->number = objValue->number;
      currentObjectArray->generation = objValue->generation;
      currentObjectArray->next->next = NULL;
      break;

    case panda_dictionaryvalue:
      // This is a sub-dictionary
      dictValue = va_arg (argPtr, panda_dictionary *);

      if (overwriting == panda_false)
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
		case panda_textvalue:
		case panda_brackettedtextvalue:
		case panda_literaltextvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
				     dictValue->valueType,
				     dictValue->textValue);
		  break;

		case panda_objectvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
				     panda_literaltextvalue,
				     dictValue->textValue);
		  break;

		case panda_dictionaryvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
				     dictValue->valueType,
				     dictValue->dictValue);
		  break;

		case panda_integervalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
				     dictValue->valueType,
				     dictValue->intValue);
		  break;

		case panda_objectarrayvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
				     dictValue->valueType,
				     dictValue->objectarrayValue);
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
panda_getdictvalue (panda_dictionary * dictValue)
{
  // Return a panda_dictionary value
  switch (dictValue->valueType)
    {
    case panda_dictionaryvalue:
      return dictValue->dictValue;
      break;

      // This line is a little scary -- we are going to make the int look like
      // a pointer for just a little while
    case panda_integervalue:
      return dictValue->intValue;
      break;

    case panda_textvalue:
    case panda_literaltextvalue:
    case panda_objectvalue:
      return dictValue->textValue;
      break;

    case panda_objectarrayvalue:
      return dictValue->objectarrayValue;
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

/******************************************************************************
DOCBOOK START

FUNCTION panda_freeobject
PURPOSE free the memory used by a previously created object

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_freeobject (panda_pdf *output, panda_object *);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function call frees the memory used by an object. It is used during the cleanup process prior to finishing closing a PDF document. Even if a user of the API has added object types to their code, they should not have to call this function, as their objects should be added to the object tree via <command>panda_addchild</command>() to ensure they are written to disc by <command>panda_writeobject</command>() having been traversed by <command>panda_traverseobjects</command>() at PDF close time.

RETURNS None

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_object *obj;

panda_init();

document = panda_open("filename.pdf", "w");
obj = panda_newobject(document, panda_object_normal);
panda_freeobject(document, obj)l
EXAMPLE END
SEEALSO panda_newobject, panda_writeobject, panda_traverseobjects, panda_addchild
DOCBOOK END
******************************************************************************/

void
panda_freeobject (panda_pdf * output, panda_object * freeVictim)
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

      panda_freedictionary (freeVictim->dict);
    }

  free (freeVictim);
}

void
panda_freedictionary (panda_dictionary * freeDict)
{
  panda_dictionary *now, *prev;
  int endoftheline = panda_true;

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

      if (endoftheline == panda_false)
	{
#if defined DEBUG
	  printf ("Free dictionary item named %s\n", now->name);
#endif

	  free (now->name);
	  if (now->textValue != NULL)
	    free (now->textValue);
	  if (now->dictValue != NULL)
	    panda_freedictionary (now->dictValue);
	}
      else
	endoftheline = panda_false;

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
	panda_freedictionary (freeDict->dictValue);

      free (freeDict);
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_writeobject
PURPOSE write a given object to disc

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_writeobject (panda_pdf *output, object *obj);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Writes all objects created with <command>panda_newobject</command>(), that have been added to the object tree with <command>panda_addchild</command>() via a call to <command>panda_traverseobjects</command>().

RETURNS None

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_object *obj;

panda_init();

document = panda_open("filename.pdf", "w");
obj = panda_newobject(document, panda_object_normal);
panda_writeobject(document, obj);
EXAMPLE END
SEEALSO panda_newobject, panda_freeobject, panda_traverseobjects, panda_addchild
DOCBOOK END
******************************************************************************/

void
panda_writeobject (panda_pdf * output, panda_object * dumpTarget)
{
  // Do all that is needed to dump a PDF object to disk
  unsigned long count, compressedLen;
  char *compressed;
  int compressResult, level;

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
	  dumpTarget->layoutstreamLength = strlen (dumpTarget->layoutstream);

	  // We determine if the stream is to be compressed, and then
	  // overwrite the old stream with the compressed one (and save
	  // the length).

	  if ((dumpTarget->cascadeproperties
	       [panda_object_property_compress] == panda_true) ||
	      (dumpTarget->localproperties
	       [panda_object_property_compress] == panda_true))
	    {

#if defined DEBUG
	      printf ("Compression enabled for this object\n");
#endif

	      // See zlib.h
	      compressedLen = dumpTarget->layoutstreamLength * 1.2 + 12;
	      compressed = panda_xmalloc (compressedLen);

	      // Determine what compression level to use
	      if (dumpTarget->localproperties
		  [panda_object_property_compress_level] !=
		  panda_default_compress_level)
		level = dumpTarget->localproperties
		  [panda_object_property_compress_level];
	      else
		level = dumpTarget->cascadeproperties
		  [panda_object_property_compress_level];

	      if (((compressResult = compress2 (compressed, &compressedLen,
						dumpTarget->layoutstream,
						dumpTarget->
						layoutstreamLength,
						level)) == Z_OK)
		  && (compressedLen < dumpTarget->layoutstreamLength))
		{
		  printf ("Compressed is %d [obj %d]\n", compressedLen,
			  dumpTarget->number);

		  // The compression worked (no error returned)
		  panda_adddictitem (dumpTarget->dict, "Filter",
				     panda_textvalue, "FlateDecode");

		  // Now we need to make the stream use the compressed one,
		  // and record the length
		  free (dumpTarget->layoutstream);
		  dumpTarget->layoutstream = compressed;
		  dumpTarget->layoutstreamLength = compressedLen;
		}
	    }

	  panda_adddictitem (dumpTarget->dict, "Length", panda_integervalue,
			     dumpTarget->layoutstreamLength);
	}

      // We cannot have a layoutstream and a binary stream in the same object
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_adddictitem (dumpTarget->dict, "Length", panda_integervalue,
			     dumpTarget->binarystreamLength);
	}

      // Make sure we deal with empty content streams probably
      else if (dumpTarget->isContents == panda_true)
	{
#if defined DEBUG
	  printf ("Forcing output of content stream\n");
#endif

	  panda_adddictitem (dumpTarget->dict, "Length", panda_integervalue,
			     0);
	  dumpTarget->layoutstream =
	    (char *) panda_xmalloc (sizeof (char) * 2);
	  dumpTarget->layoutstream = panda_xsnprintf (" ");
	  dumpTarget->layoutstreamLength = 1;
	}

      // We are going to dump the named object (and only the named object) to 
      // disk
      panda_printf (output, "%d %d obj\n",
		    dumpTarget->number, dumpTarget->generation);

      panda_writedictionary (output, dumpTarget, dumpTarget->dict);

      // Do we have a layoutstream?
      if (dumpTarget->layoutstream != NULL)
	{
	  panda_print (output, "stream\n");

	  for (count = 0; count < dumpTarget->layoutstreamLength; count++)
	    panda_putc (output, dumpTarget->layoutstream[count]);

	  panda_print (output, "\nendstream\n");
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
panda_writedictionary (panda_pdf * output, panda_object * obj,
		       panda_dictionary * incoming)
{
  // Recursively write the dictionary out (including sub-dictionaries)
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictNow;
  int atBegining = panda_true;
  panda_child *currentKid;

#if defined DEBUG
  printf ("Starting to write a dictionary\n");
#endif

  // The start of the dictionary
  panda_print (output, "<<\n");

  // Enumerate the dictionary elements
  dictNow = incoming;

  while (dictNow->next != NULL)
    {
      switch (dictNow->valueType)
	{
	case panda_textvalue:
	case panda_objectvalue:
	case panda_literaltextvalue:
	case panda_brackettedtextvalue:
#if defined DEBUG
	  printf ("Writing a text value named %s into the dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %s\n", dictNow->name,
			dictNow->textValue);

	  // If the type is type, then possibly output the Kids line for the 
	  // pages object
	  if ((strcmp (dictNow->name, "Type") == 0)
	      && (obj->isPages == panda_true))
	    {
	      panda_print (output, "\t/Kids [");

	      // Do the dumping
	      currentKid = obj->children;

	      while (currentKid->next != NULL)
		{
		  if (atBegining == panda_false)
		    panda_print (output, " ");
		  else
		    atBegining = panda_false;

		  if(currentKid->me->isTemplate == panda_false)
		    panda_printf (output, "%d %d R",
				  currentKid->me->number,
				  currentKid->me->generation);
		  else atBegining = panda_true;
		  
		  // Next
		  currentKid = currentKid->next;
		}

	      // End it all
	      panda_print (output, "]\n");
	    }
	  break;

	case panda_integervalue:
#if defined DEBUG
	  printf ("Writing a int value with name %s into the dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %d\n", dictNow->name,
			dictNow->intValue);
	  break;

	case panda_objectarrayvalue:
#if defined DEBUG
	  printf
	    ("Writing an object array value with name %s into dictionary\n",
	     dictNow->name);
#endif

	  // Start the array in the file
	  atBegining = panda_true;

	  panda_printf (output, "\t/%s [", dictNow->name);

	  // Go through the array list until the end
	  currentObjectArray = dictNow->objectarrayValue;
	  while (currentObjectArray->next != NULL)
	    {
	      if (atBegining == panda_true)
		atBegining = panda_false;
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

	case panda_dictionaryvalue:
	  // These are handled recursively
	  if (dictNow->dictValue == NULL)
	    panda_error ("Subdictionary value erroneously NULL.");

#if defined DEBUG
	  printf ("Output the subdictionary starting with the name %s\n",
		  dictNow->dictValue->name);
#endif

	  panda_printf (output, "\t/%s ", dictNow->name);

	  panda_writedictionary (output, output->dummyObj,
				 dictNow->dictValue);
	  break;

	default:
	  panda_error ("Unknown dictionary type");
	}

      dictNow = dictNow->next;
    }

  panda_print (output, ">>\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_addchild
PURPOSE add an object to the object tree

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_addchild (panda_object *parent, panda_object *child);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. Once an object has been created with <command>panda_newobject</command>() it is added to the object tree with this call. This ensures it is written out to disc via <command>panda_writeobject</command>() when <command>panda_close</command>() is called.

RETURNS None

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_object *obj, *obj2;

panda_init();

document = panda_open("filename.pdf", "w");
obj = panda_newobject(document, panda_object_normal);
obj2 = panda_newobject(document, panda_object_normal);
panda_addhild(obj, obj2);
EXAMPLE END
SEEALSO panda_newobject, panda_freeobject, panda_writeobject, panda_traverseobjects
DOCBOOK END
******************************************************************************/

void
panda_addchild (panda_object * parentObj, panda_object * childObj)
{
  panda_child *currentChild = parentObj->children;

  // Find the end of the list of children
  if (parentObj->cachedLastChild != NULL)
    currentChild = parentObj->cachedLastChild;

  while (currentChild->next != NULL)
    currentChild = currentChild->next;

  // Make a new end
  currentChild->next = (panda_child *) panda_xmalloc (sizeof (panda_child));
  currentChild->next->next = NULL;

  // Make me be the child object
  currentChild->me = childObj;

  // Cascade the relevant properties
  memcpy (childObj->cascadeproperties, parentObj->cascadeproperties,
	  panda_object_property_max);

  // Cache it
  parentObj->cachedLastChild = currentChild;
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_traverseobjects
PURPOSE traverse the PDF object tree and perform an operation

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_traverseobjects (panda_pdf * output, panda_object * startObject, int direction, traverseFunct function);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function traverses the object tree created by <command>panda_addchild</command>() and repeatedly calls the function defined as a callback. The traversal has a direction as defined by: panda_up (bottom up) or panda_down (top down). This call is commonly used by <command>panda_close</command> to call <command>panda_writeobject</command>() and <command>panda_freeobject</command>(). API users might also find it useful, although I wouldn't know why.

RETURNS None

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;

panda_init();

document = panda_open("filename.pdf", "w");

... create a whole bunch of objects and add them to the tree ...
panda_traverseobjects(document, document->catalog, panda_down, panda_writeobject);
EXAMPLE END
SEEALSO panda_newobject, panda_freeobject, panda_writeobject, panda_addchild
DOCBOOK END
******************************************************************************/

void
panda_traverseobjects (panda_pdf * output, panda_object * dumpTarget,
		       int direction, traverseFunct function)
{
  // Write out an object and all of it's children. This may be done with
  // recursive calls and writeobject()
  panda_child *currentChild;

  // No children
  if (((panda_child *) dumpTarget->children)->next == NULL)
    {
      function (output, dumpTarget);
      return;
    }

  // Otherwise, for me and then for all children
  if (direction == panda_down)
    function (output, dumpTarget);

  currentChild = dumpTarget->children;
  while (currentChild->next != NULL)
    {
      panda_traverseobjects (output, currentChild->me, direction, function);
      currentChild = currentChild->next;
    }

  if (direction == panda_up)
    function (output, dumpTarget);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_setobjectproperty
PURPOSE set a property value for an object

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_setobjectproperty (panda_object *target, int scope, int propid, int propval);
SYNOPSIS END

DESCRIPTION Properties are a way of specifing things about objects. These properties can have either a cascade scope (they affect all subsequently created objects that are children of that object) -- <command>panda_scope_cascade</command>, or local (they only occur for that object) -- <command>panda_scope_local</command>. Possible properties are defined in the <command>panda_const_properties</command> manual page.

RETURNS None

EXAMPLE START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;

panda_pdf *document;
panda_object *obj;

panda_init();

document = panda_open("filename.pdf", "w");
obj = panda_newobject(document, panda_object_normal);
panda_setproperty(obj, panda_scope_cascade, panda_object_property_compress, panda_true);

EXAMPLE END
SEEALSO panda_newobject, panda_const_properties
DOCBOOK END
******************************************************************************/

void
panda_setproperty (panda_object * target, int scope, int key, int value)
{
#if defined DEBUG
  printf ("Set a property for object %d %d\n", target->number,
	  target->generation);
#endif

  if ((key < 0) || (key > panda_object_property_max))
    {
#if defined DEBUG
      printf ("Undefined key value specifiec\n");
#endif
      return;
    }

  switch (scope)
    {
    case panda_scope_cascade:
#if defined DEBUG
      printf ("Cascade\n");
#endif
      target->cascadeproperties[key] = value;
      break;

    case panda_scope_local:
#if defined DEBUG
      printf ("Local\n");
#endif
      target->localproperties[key] = value;
      break;

    default:
      // Do nothing
    }
}
