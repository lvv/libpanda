/******************************************************************************
  Database.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      12012002

  Purpose:
    With the new improved and somewhat shiny database backend to Panda, then
    we need to interact with the backend database. This file does all of that
    interaction, saving us from having to do it elsewhere. It also makes it
    easier to change the database later if I need to...
******************************************************************************/

#if defined _WINDOWS
#include "panda/constants.h"
#include "panda/functions.h"
#else
#include <panda/constants.h>
#include <panda/functions.h>
#endif

/******************************************************************************
DOCBOOK START

FUNCTION panda_dbopen
PURPOSE prepare the Panda internal database for use

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_dbopen(panda_pdf *document);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This command opens the TDB database which is used to store information about things like dictionary items which have been created in the document. The use of this database makes Panda much more reliable than it has previously been.

RETURNS Nothing

EXAMPLE START
There is no examples for this internal call
EXAMPLE END
SEEALSO panda_dbclose, panda_dbwrite, panda_dbread
DOCBOOK END
******************************************************************************/

void
panda_dbopen (panda_pdf * document)
{
#if defined DEBUG
  printf ("Opening the database\n");
#endif

  document->db = tdb_open ("panda.tdb", 0,
			   TDB_CLEAR_IF_FIRST, O_RDWR | O_CREAT | O_TRUNC,
			   0600);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_dbclose
PURPOSE finalize the use of the Panda internal database

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_dbclose(panda_pdf *document);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This command closes the TDB database which is used to store information about things like dictionary items which have been created in the document. The use of this database makes Panda much more reliable than it has previously been.

RETURNS Nothing

EXAMPLE START
There is no examples for this internal call
EXAMPLE END
SEEALSO panda_dbopen, panda_dbwrite, panda_dbread
DOCBOOK END
******************************************************************************/

void
panda_dbclose (panda_pdf * document)
{
#if defined DEBUG
  printf ("Closing the database\n");
#endif

  tdb_close (document->db);
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_dbwrite
PURPOSE write a value into the Panda internal database

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
void panda_dbwrite(panda_pdf *document, char *key, char *value);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function write a value into the Panda internal database.

RETURNS Nothing

EXAMPLE START
There is no examples for this internal call
EXAMPLE END
SEEALSO panda_dbopen, panda_dbclose, panda_dbread
DOCBOOK END
******************************************************************************/

void
panda_dbwrite (panda_pdf * document, char *key, char *value)
{
  TDB_DATA dbkey, dbdata;

#if defined DEBUG
  printf ("Storing (%s, %s) in 0x%08x\n", key, value, document->db);
#endif

  if (key == NULL)
    panda_error (panda_true, "Cannot store null key\n");
  if (value == NULL)
    panda_error (panda_true, "Cannot store null value\n");

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;
  dbdata.dptr = value;
  dbdata.dsize = strlen (value) + 1;

  if (tdb_store (document->db, dbkey, dbdata, TDB_REPLACE) != 0)
    {
      panda_error (panda_true, "Database error");
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION panda_dbread
PURPOSE read a value from the Panda internal database

SYNOPSIS START
#include&lt;panda/constants.h&gt;
#include&lt;panda/functions.h&gt;
char *panda_dbread(panda_pdf *document, char *key);
SYNOPSIS END

DESCRIPTION <command>PANDA INTERNAL</command>. This function reads a value from the Panda internal database.

RETURNS The value, or NULL if there is no value.

EXAMPLE START
There is no examples for this internal call
EXAMPLE END
SEEALSO panda_dbopen, panda_dbclose, panda_dbread
DOCBOOK END
******************************************************************************/
char *
panda_dbread (panda_pdf * document, char *key)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL)
    panda_error (panda_true, "Cannot read a NULL key\n");

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;

  dbdata = tdb_fetch (document->db, dbkey);

#if defined DEBUG
  printf ("Getting %s returned %s\n", key, dbdata.dptr);
#endif

  return dbdata.dptr;
}
