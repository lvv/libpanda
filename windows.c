/******************************************************************************
  Windows.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062001
	Michael Still    Complete change of purpose -- now windows db      22022002

  Purpose:
    This file implements a _very_ simple database to replace TDB on Windows.
	It doesn't have much future -- I would much rather port TDB to Windows when
	I have the time. I would love patches which do that. The other thing I
	wouldn't mind is some simple patches to make this run faster (it is very
	bloody minded at the moment).

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

#include "stdafx.h"
typedef struct panda_internal_windb 
{
  char *key;
   char *value;
   struct panda_internal_windb *next;
 }
 panda_windb;
void
panda_windbopen (panda_pdf * document) 
{
  document->db = panda_xmalloc (sizeof (panda_windb));
  ((panda_windb *) document->db)->next = NULL;
  ((panda_windb *) document->db)->key = NULL;
  ((panda_windb *) document->db)->value = NULL;
  if (((panda_windb *) document->db)->next != NULL)
    panda_error (panda_true, "Could not set a database pointer to NULL");
}
void
panda_windbclose (panda_pdf * document) 
{
  panda_windb * now, *nextnow;
  now = document->db;
  while (now->next != NULL)
    
    {
      nextnow = now->next;
      panda_xfree (now->key);
      panda_xfree (now->value);
      panda_xfree (now);
      now = nextnow;
    }
  panda_xfree (now);
}
void
panda_windbwrite (panda_pdf * document, char *key, char *value) 
{
  panda_windb * now;
  
    // Find the item, or the end -- I have to do it this way so that Visual Studio doesn't
    // complain
    now = (panda_windb *) document->db;
  while (now->next != NULL)
    
    {
      if (strcmp (now->key, key) == 0)
	break;
      now = now->next;
    }
  
    // We are appending to the chain
    if (now->next == NULL)
    
    {
      now->next = (panda_windb *) panda_xmalloc (sizeof (panda_windb));
      now->next->next = NULL;
      now->next->key = NULL;
      now->next->value = NULL;
      now->key = panda_xsnprintf ("%s", key);
      now->value = panda_xsnprintf ("%s", value);
      return;
    }
  
    // We are overwriting an existing value (a memory leak for the moment)
    now->value = panda_xsnprintf ("%s", value);
}
char *
panda_windbread (panda_pdf * document, char *key) 
{
  panda_windb * now;
  
    // Find the item, or the end
    now = (panda_windb *) document->db;
  while (now->next != NULL)
    
    {
      if (strcmp (now->key, key) == 0)
	break;
      now = now->next;
    }
  
    // now->next = NULL means at the end of the list
    return (now->next == NULL) ? NULL : panda_xsnprintf ("%s", now->value);
}
