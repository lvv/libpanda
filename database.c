/******************************************************************************
  Database.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      12012002

  Purpose:
    With the new improved and somewhat shiny database backend to Panda, then
    we need to interact with the backend database. This file does all of that
    interaction, saving us from having to do it elsewhere. It also makes it
    easier to change the database later if I need to...

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
#else
#include <panda/constants.h>
#include <panda/functions.h>
#include "config.h"

#ifdef USE_EDB
#include "contrib/Edb.h"
#else
#include <db.h>
#endif

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
#ifdef DEBUG
  printf ("Opening the database\n");
#endif // DEBUG

#ifdef _WINDOWS
    panda_windbopen (document);
#else
#ifdef USE_EDB
    if ( !((E_DB_File *)document->db = e_db_open("panda.db")) )
      panda_error(panda_true, "Could not open database.");
#else
    int ec;

    if ((ec = db_create(((DB **)&(document->db)), NULL, 0)) != 0)
      panda_error(panda_true, panda_xsnprintf("Could not open database: %s", db_strerror(ec)));
    if ((ec = ((DB *)document->db)->open(((DB *)document->db), NULL, "panda.db", NULL, DB_BTREE, DB_CREATE, 0664)) != 0)
      panda_error(panda_true, panda_xsnprintf("Could not open database: %s", db_strerror(ec)));
#endif // USE_EDB
#endif // _WINDOWS
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

#ifdef _WINDOWS
    panda_windbclose (document);
#else
#ifdef USE_EDB
    e_db_close((E_DB_File *)document->db);
#else
    ((DB *)document->db)->close((DB *)document->db, 0);
#endif // USE_EDB
#endif // _WINDOWS
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
#if defined _WINDOWS
    panda_windbwrite (document, key, value);
#else

#if defined DEBUG
  printf ("Storing (%s, %s) in 0x%08x\n", key, value, document->db);
#endif

  if (key == NULL)
    panda_error (panda_true, "Cannot store null key\n");
  if (value == NULL)
    panda_error (panda_true, "Cannot store null value\n");

#ifdef USE_EDB

  e_db_data_set((E_DB_File *)document->db, key, value, strlen(value)+1);
#else
  DBT db_key, db_data;
  int ec;

  memset(&db_key, 0, sizeof(db_key));
  memset(&db_data, 0, sizeof(db_data));
  db_key.data = key;
  db_key.size = strlen(key) + 1;
  db_data.data = value;
  db_data.size = strlen(value) + 1;

  if ((ec = ((DB *)document->db)->put(((DB *)document->db), NULL, &db_key, &db_data, 0)) != 0)
    panda_error(panda_true, panda_xsnprintf("Could not store data in database: %s", db_strerror(ec)));
#endif // USE_EDB
#endif // _WINDOWS
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
#if defined _WINDOWS
  return panda_windbread (document, key);
#else
#ifdef USE_EDB
  int size;

  if (key == NULL)
    panda_error (panda_true, "Cannot read a NULL key\n");

  return e_db_data_get((E_DB_File *)document->db, key, &size);
#else
  DBT db_key, db_data;
  int ec;

  memset(&db_key, 0, sizeof(db_key));
  memset(&db_data, 0, sizeof(db_data));
  db_key.data = key;
  db_key.size = strlen(key)+1;

  if ( (ec = ((DB *)document->db)->get(((DB *)document->db), NULL, &db_key, &db_data, 0)) == 0 )
    return panda_xsnprintf("%s", db_data.data);
  else
    return NULL;
#endif // USE_EDB
#endif // _WINDOWS
}
