/******************************************************************************
  Objects.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda is based on the concept of objects. This file contains all of
    methods needed to maintain the objects that we have.

  Copyright (C) Michael Still 2000 - 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#if defined _WINDOWS
#include "stdafx.h"

#include "contrib/libz/zlib.h"
#else
#include <panda/constants.h>
#include <panda/functions.h>

#include <zlib.h>
#endif

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
  int dictno;
  char *dbkey, *dbdata;

  // Do all the magic needed to create an object
  panda_object *created;

  // Get some memory
  created = (panda_object *) panda_xmalloc (sizeof (panda_object));
  doc->totalObjectNumber++;

  // Initialise the object number
  if (type == panda_placeholder)
    created->number = -doc->nextPHObjectNumber++;
  else
    created->number = doc->nextObjectNumber++;

  // Create the dictionary for this object in the database
  dictno = panda_adddict (doc);
  dbkey = panda_xsnprintf ("obj-%d-dictno", created->number);
  dbdata = panda_xsnprintf ("%d", dictno);
  panda_dbwrite (doc, dbkey, dbdata);
  panda_xfree (dbkey);
  panda_xfree (dbdata);

  // We have no children at the moment
  created->children = (panda_child *) panda_xmalloc (sizeof (panda_child));

  ((panda_child *) created->children)->next = NULL;
  (panda_child *) created->cachedLastChild = NULL;

  // By default this object is not a pages object
  created->isPages = panda_false;
  created->isPlaceholder =
    type == panda_placeholder ? panda_true : panda_false;

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

  if (type == panda_placeholder)
    {
      return created;
    }

  // New objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new object to the end of the linked list that we use to append
  // the xref table onto the end of the PDF
  doc->xrefTail->me = created;

  // Make space for the next one
  doc->xrefTail->next = panda_xmalloc (sizeof (panda_xref));
  doc->xrefTail->next->next = NULL;
  doc->xrefTail = doc->xrefTail->next;

  // Set the value of the local and cascaded properties for this object to 
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

// Create a new dictionary
int
panda_adddict (panda_pdf * document)
{
  char *dbkey, *dbdata;
  int max;

  // Determine how many dictionaries we currently have
#if defined DEBUG
  printf ("Determining how many dictionaries are currently defined\n");
#endif

  if ((dbdata = panda_dbread (document, "dict-count")) == NULL)
    {
      max = 0;
    }
  else
    {
      max = atoi (dbdata);
      panda_xfree (dbdata);
    }

  // Increment that count
  dbdata = panda_xsnprintf ("%d", ++max);
  panda_dbwrite (document, "dict-count", dbdata);
  panda_xfree (dbdata);
  return max;
}

// The value we return here is the database key that the dictionary value
// was stored at, if this value is used as the name of a later key, then
// this implies that we are adding a subdictionary. The valuetype for this
// first key should have been panda_dictvalue. No longer return a value...
void
panda_adddictitem (panda_pdf * document, panda_object * input, char *name,
		   int valueType, ...)
{
  // Add an item to the dictionary in the object
  int dictno, dictelem;
  char *dbdata, *dbkey, *dictdata, *retval;
  va_list argPtr;
  panda_object *obj;

#if defined DEBUG
  printf ("Adddictitem called for object numbered %d\n", input->number);
#endif

  // Get this object's dictionary number
  dictno = panda_getobjdictno (document, input);

  // Gain access like variable
  va_start (argPtr, valueType);

  // Before we can find where to put the data, we need to turn the
  // data into something which we can store
  switch (valueType)
    {
    case panda_objectarrayvalue:
      obj = (panda_object *) va_arg (argPtr, panda_object *);
      dictdata = panda_xsnprintf ("%d %d R", obj->number, obj->generation);
      break;

    case panda_textvalue:
      dictdata = panda_xsnprintf ("/%s", (char *) va_arg (argPtr, char *));
      break;

    case panda_literaltextvalue:
      dictdata = panda_xsnprintf ("%s", (char *) va_arg (argPtr, char *));
      break;

    case panda_brackettedtextvalue:
      dictdata = panda_xsnprintf ("(%s)", (char *) va_arg (argPtr, char *));
      break;

    case panda_integervalue:
      dictdata = panda_xsnprintf ("%d", (int) va_arg (argPtr, int));
      break;

    case panda_objectvalue:
      obj = (panda_object *) va_arg (argPtr, panda_object *);
      dictdata = panda_xsnprintf ("%d %d R", obj->number, obj->generation);
      break;

    case panda_booleanvalue:
      dictdata = panda_xsnprintf ("%s",
				  (((int) va_arg (argPtr, int)) ==
				   panda_true) ? "true" : "false");
      break;

    case panda_doublevalue:
      dictdata = panda_xsnprintf ("%f", (double) va_arg (argPtr, double));
      break;
    }

  // Store in the dictionary
  dictelem = panda_getdictelem (document, dictno, name);
  retval = panda_adddictiteminternal (document, dictno, dictelem,
				      name, valueType, dictdata);
  panda_xfree (dictdata);
  panda_xfree (retval);
}

// Insert data into the dictionary at a given location
char *
panda_adddictiteminternal (panda_pdf * document, int passeddictno,
			   int dictelem, char *name, int valueType,
			   char *value)
{
  char *dbkey, *dbdata, *dbdata2, *dbname, *tempname, *namebit, *therest,
    *retval;
  int dictno, pdictelem, pdictno;

#if defined DEBUG
  printf ("Add dictionary item internally (name %s, dict %d, element %d)\n",
	  name, passeddictno, dictelem);
#endif

  // We need to determine if the key we have been handed really refers to
  // a subdictionary
  tempname = panda_xsnprintf ("%s", name);
  namebit = strtok (tempname, "/");
  therest = strtok (NULL, "");

  // If this is a dictionary reference, then we should do that instead
  if ((therest != NULL) && (strlen (therest) > 0))
    {
      // Get the dictionary number for this sub item
      dbkey = panda_finddictiteminternal (document, passeddictno, namebit);
      if (dbkey != NULL)
	{
	  dbdata = panda_dbread (document, dbkey);
	  panda_xfree (dbkey);
	}
      else
	{
	  dbdata = NULL;
	}

      if (dbdata != NULL)
	{
	  dictno = atoi (dbdata);
	  panda_xfree (dbdata);
	}
      else
	{
#if defined DEBUG
	  printf ("Needed to create a new subdictionary\n");
#endif

	  // We need to add the dictionary item here
	  pdictno = panda_adddict (document);
	  dbdata = panda_xsnprintf ("%d", pdictno);
	  retval = panda_adddictiteminternal (document, passeddictno,
					      panda_getdictelem (document,
								 passeddictno,
								 namebit),
					      namebit, panda_dictionaryvalue,
					      dbdata);

	  panda_xfree (dbdata);
	  panda_xfree (retval);
	  dictno = pdictno;
	}

#if defined DEBUG
      printf ("Create a subdictionary\n");
#endif

      // We need the element number for the dictionary
      dictelem = panda_getdictelem (document, dictno, therest);
      retval = panda_adddictiteminternal (document, dictno, dictelem,
					  therest, valueType, value);
      panda_xfree (tempname);

#if defined DEBUG
      printf ("Rolling up sudictionary creation\n");
#endif

      return retval;
    }
  else
    dictno = passeddictno;

  // Otherwise, we do some stuff
#if defined DEBUG
  printf ("Storing a dictionary item: dict = %d, element = %d\n",
	  dictno, dictelem);
#endif

  dbkey = panda_xsnprintf ("dict-%d-%d-name", dictno, dictelem);
  panda_dbwrite (document, dbkey, name);
  panda_xfree (dbkey);

  dbkey = panda_xsnprintf ("dict-%d-%d-type", dictno, dictelem);
  dbdata = panda_xsnprintf ("%d", valueType);
  panda_dbwrite (document, dbkey, dbdata);
  panda_xfree (dbkey);
  panda_xfree (dbdata);

  // If this is a panda_objectarrayvalue, then we append to the end
  dbkey = panda_xsnprintf ("dict-%d-%d-value", dictno, dictelem);
  if (valueType == panda_objectarrayvalue)
    {
      dbdata = panda_dbread (document, dbkey);

      if (dbdata != NULL)
	dbdata2 = panda_xsnprintf ("%s %s", dbdata, value);
      else
	dbdata2 = panda_xsnprintf ("%s", value);
    }
  else
    dbdata2 = panda_xsnprintf ("%s", value);

  panda_dbwrite (document, dbkey, dbdata2);

  panda_xfree (tempname);
  panda_xfree (dbkey);
  panda_xfree (dbdata2);

  // todo_mikal: I think there should be a free here, dmalloc disagrees
  //if(dbdata != NULL)
  //  free(dbdata);

#if defined DEBUG
  printf ("Finished inserting dictionary item\n");
#endif

  return panda_xsnprintf ("dict-%d-%d-", dictno, dictelem);
}

// Get the dictionary number associated with an object
int
panda_getobjdictno (panda_pdf * document, panda_object * input)
{
  int dictno;
  char *dbkey, *dbdata;

#if defined DEBUG
  printf ("Getting the dictionary number for the object numbered %d\n",
	  input->number);
#endif

  // We need to get the dictionary number for this object
  dbkey = panda_xsnprintf ("obj-%d-dictno", input->number);
  if ((dbdata = panda_dbread (document, dbkey)) == NULL)
    {
      panda_error (panda_true, "Object lacks a dictionary");
    }
  panda_xfree (dbkey);

  // Now we can get the count of the current entries in the dictionary
  dictno = atoi (dbdata);
  panda_xfree (dbdata);

  if (dictno == 0)
    panda_error (panda_true, "Object does not have a dictionary\n");

  return dictno;
}

// Find the dictionary element we are looking for
int
panda_getdictelem (panda_pdf * document, int dictno, char *name)
{
  int count;
  char *dbkey, *dbdata;

#if defined DEBUG
  printf ("Searching for a dictionary element named %s in dictionary %d\n",
	  name, dictno);
#endif

  if (dictno == 0)
    panda_error (panda_true, "Invalid dictionary number\n");

  count = 0;
  do
    {
      dbkey = panda_xsnprintf ("dict-%d-%d-name", dictno, count);
      dbdata = panda_dbread (document, dbkey);

      // Do the names match?
      if ((dbdata != NULL) && (strcmp (name, dbdata) == 0))
	{
	  return count;
	}

      panda_xfree (dbkey);
      panda_xfree (dbdata);
      count++;

#if defined DEBUG
      if (count > 1000)
	panda_error (panda_true, "Counter too big!\n");
#endif
    }
  while (dbdata != NULL);

  // -1 because of the increment above
  return count - 1;
}

// Find a given dictionary item, and return the database key for the value 
// element
char *
panda_finddictitem (panda_pdf * document, panda_object * input, char *name)
{
  int dictno;
  char *dbdata, *dbkey;

#if defined DEBUG
  printf ("Finding a dictionary item for the object numbered %d\n",
	  input->number);
#endif

  // We need to get the dictionary number for this object
  dictno = panda_getobjdictno (document, input);
  return panda_finddictiteminternal (document, dictno, name);
}

// Internal version of the find
char *
panda_finddictiteminternal (panda_pdf * document, int dictno, char *name)
{
  char *dbdata, *dbkey;
  int count;

#if defined DEBUG
  printf ("Searching for %s in dictionary %d\n", name, dictno);
#endif

  // We can now go through the dictionary elements and see if we are
  // clobbering this one
  count = 0;
  do
    {
      dbkey = panda_xsnprintf ("dict-%d-%d-name", dictno, count);
      dbdata = panda_dbread (document, dbkey);	// NULL here is ok

      // Do the names match?
      if ((dbdata != NULL) && (strcmp (name, dbdata) == 0))
	{
	  panda_xfree (dbdata);
	  panda_xfree (dbkey);
	  dbkey = panda_xsnprintf ("dict-%d-%d-value", dictno, count);
	  return dbkey;
	}

      panda_xfree (dbkey);
      panda_xfree (dbdata);

#if defined DEBUG
      if (count > 1000)
	panda_error (panda_true, "Counter too big!\n");
#endif

      count++;
    }
  while (dbdata != NULL);

  return NULL;
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
  panda_freeobjectactual (output, freeVictim, panda_true, panda_true);
}

void
panda_freetempobject (panda_pdf * output, panda_object * freeVictim,
		      int freedict)
{
  panda_freeobjectactual (output, freeVictim, freedict, panda_true);
}

void
panda_freeobjectactual (panda_pdf * output, panda_object * freeVictim,
			int freedict, int freekids)
{
#if defined DEBUG
  printf ("Freeing object number %d\n", freeVictim->number);
#endif

  // Skip placeholder objects
  if (freeVictim->number != -1)
    {
#if defined DEBUG
      printf ("Actually freeing this object (not a placeholder)\n");
#endif

      panda_xfree (freeVictim->layoutstream);
      panda_xfree (freeVictim->binarystream);

      panda_xfree (freeVictim->currentSetFont);
    }

  if (freekids == panda_true)
    panda_xfree (freeVictim->children);

  panda_xfree (freeVictim);
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

  // We don't dump place holder objects
  if (dumpTarget->isPlaceholder != panda_true)
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
#if defined DEBUG
	  printf ("Processing the layoutstream\n");
#endif
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
		  panda_adddictitem (output, dumpTarget, "Filter",
				     panda_textvalue, "FlateDecode");

		  // Now we need to make the stream use the compressed one,
		  // and record the length
		  panda_xfree (dumpTarget->layoutstream);
		  dumpTarget->layoutstream = compressed;
		  dumpTarget->layoutstreamLength = compressedLen;
		}
	    }

#if defined DEBUG
	  printf ("Writing out the length of the stream\n");
#endif

	  panda_adddictitem (output, dumpTarget, "Length", panda_integervalue,
			     dumpTarget->layoutstreamLength);

#if defined DEBUG
	  printf ("Finished with layoutstream\n");
#endif
	}

      // We cannot have a layoutstream and a binary stream in the same object
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_adddictitem (output, dumpTarget, "Length", panda_integervalue,
			     dumpTarget->binarystreamLength);
	}

      // Make sure we deal with empty content streams probably
      else if (dumpTarget->isContents == panda_true)
	{
#if defined DEBUG
	  printf ("Forcing output of content stream\n");
#endif

	  panda_adddictitem (output, dumpTarget, "Length", panda_integervalue,
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

#if defined DEBUG
      printf ("Writing out the dictionary\n");
#endif

      panda_writedictionary (output, dumpTarget);

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
panda_writedictionary (panda_pdf * output, panda_object * obj)
{
  int dictno;

#if defined DEBUG
  printf ("Writing out the dictionary items for the object numbered %d\n",
	  obj->number);
#endif

  // Recursively write the dictionary out (including sub-dictionaries)
  dictno = panda_getobjdictno (output, obj);
  panda_writedictionaryinternal (output, dictno, 1);
}

// Dump a specific dictionary to the PDF file
void
panda_writedictionaryinternal (panda_pdf * output, int dictno, int depth)
{
  int count, dcount;
  char *dbkey, *dbname = NULL, *dbvalue, *dbtype;

#if defined DEBUG
  printf ("Internal call to write the dictionary\n");
#endif

  panda_print (output, "<<\n");

  // We can now go through the dictionary elements and see if we are
  // clobbering this one
  count = 0;
  do
    {
      panda_xfree (dbname);

      dbkey = panda_xsnprintf ("dict-%d-%d-name", dictno, count);
      dbname = panda_dbread (output, dbkey);
      panda_xfree (dbkey);

      dbkey = panda_xsnprintf ("dict-%d-%d-value", dictno, count);
      dbvalue = panda_dbread (output, dbkey);
      panda_xfree (dbkey);

      dbkey = panda_xsnprintf ("dict-%d-%d-type", dictno, count);
      dbtype = panda_dbread (output, dbkey);
      panda_xfree (dbkey);

      if (dbname != NULL)
	{
	  for (dcount = 0; dcount < depth; dcount++)
	    panda_printf (output, "\t");
	  panda_printf (output, "/%s ", dbname);

	  if (atoi (dbtype) == panda_dictionaryvalue)
	    panda_writedictionaryinternal (output, atoi (dbvalue), depth + 1);
	  else if (atoi (dbtype) == panda_objectarrayvalue)
	    panda_printf (output, "[%s]\n", dbvalue);
	  else
	    panda_printf (output, "%s\n", dbvalue);

	  panda_xfree (dbtype);
	  panda_xfree (dbvalue);
	}

#if defined DEBUG
      if (count > 1000)
	panda_error (panda_true, "Counter too big!\n");
#endif

      count++;
    }
  while (dbname != NULL);

  for (dcount = 0; dcount < depth - 1; dcount++)
    panda_print (output, "\t");
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

#if defined DEBUG
  printf ("Cleaning up object number %d\n", dumpTarget->number);
#endif

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
      // Do nothing -- Windows requires the semi colon for some reason
      ;
    }
}
