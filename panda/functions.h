/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#include "objects.h"

#ifndef PANDA_FUNCTIONS_H
#define PANDA_FUNCTIONS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if defined DMALLOC
#include <dmalloc.h>
#endif

// A type needed for panda_traverseobjects
typedef void (*traverseFunct) (pdf *, object *);

/******************************************************************************
  Date.c
******************************************************************************/

char *panda_makedate (int, int, int, int, int, int);
char *panda_nowdate ();

/******************************************************************************
  Error.c
******************************************************************************/

void panda_error (char *);

/******************************************************************************
  Images.c

  - panda_panda_imageboxrot concept by Ceasar Miquel (miquel@df.uba.ar), modified by Mikal
******************************************************************************/

void panda_imagebox (pdf *, page *, int, int, int, int, char *, int);
void panda_imageboxrot (pdf *, page *, int, int, int, int, double, char *, int);
void panda_insertTIFF (pdf *, page *, object *, char *);
void panda_insertJPEG (pdf *, page *, object *, char *);
void panda_insertPNG (pdf *, page *, object *, char *);

/******************************************************************************
  Info.c
******************************************************************************/

void panda_checkinfo (pdf *);
void panda_setauthor (pdf *, char *);
void panda_setcreator (pdf *, char *);
void panda_settitle (pdf *, char *);
void panda_setsubject (pdf *, char *);
void panda_setkeywords (pdf *, char *);

/******************************************************************************
  Font.c
******************************************************************************/

char *panda_createfont (pdf *, char *, int, char *);
void panda_setfont (pdf *, char *);
void panda_panda_setfontsize (pdf *, int);
object *panda_getfontobj (pdf *, char *);
void panda_panda_setfontmode (pdf *, int);
void panda_setcharacterspacing (pdf *, double);
void panda_setwordspacing (pdf *, double);
void panda_sethorizontalscaling (pdf *, double);
void panda_setleading (pdf *, double);

/******************************************************************************
  Memory.c
******************************************************************************/

  void *panda_xmalloc(size_t);

/******************************************************************************
  Panda.c
******************************************************************************/

void panda_init ();
pdf *panda_open (char *, char *);
pdf *panda_open_suppress (char *, char *);
pdf *panda_open_actual (char *, char *, int);
void panda_close (pdf *);
page *panda_page (pdf *, char *);
void panda_closetext (pdf *, object *);

/******************************************************************************
  Objects.c
******************************************************************************/

object *panda_newobject (pdf *, int);
dictionary *panda_adddictitem (dictionary *, char *, int, ...);
void *panda_panda_getdictvalue (dictionary *);
dictionary *panda_getdict (dictionary *, char *);
void panda_freeobject (pdf *, object *);
void panda_freedictionary (dictionary *);
void panda_writeobject (pdf *, object *);
void panda_writedictionary (pdf *, object *, dictionary *);
void panda_addchild (object *, object *);
void panda_traverseobjects (pdf *, object *, int, traverseFunct);

/******************************************************************************
  Text.c
******************************************************************************/

void panda_textbox (pdf *, page *, int, int, int, int, char *);

/******************************************************************************
  Trailer.c
******************************************************************************/

void panda_writetrailer (pdf *);

/******************************************************************************
  Utility.c
******************************************************************************/

void panda_printf (pdf *, char *, ...);
char *panda_streamprintf (char *, char *, ...);
void panda_putc (pdf *, int c);
void panda_print (pdf *, char *);

/******************************************************************************
  XREF.c
******************************************************************************/

void panda_writexref (pdf *);

#ifdef __cplusplus
}
#endif

#endif /* PANDA_FUNCTIONS_H */
