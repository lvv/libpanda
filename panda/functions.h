/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#include <panda/objects.h>

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

// A type needed for panda_traversepanda_objects
typedef void (*traverseFunct) (panda_pdf *, panda_object *);

/******************************************************************************
  Curves.c
******************************************************************************/

void panda_setlinestart(panda_page *, int, int);
void panda_addlinesegment(panda_page *, int, int);
void panda_addcubiccurvesegment(panda_page *, int, int, int, int, int, int);
void panda_addquadraticcurvesegment(panda_page *, int, int, int, int);
void panda_closeline(panda_page *);
void panda_endline(panda_page *);

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

  - panda_imageboxrot concept by Ceasar Miquel (miquel@df.uba.ar), modified by Mikal
******************************************************************************/

void panda_imagebox (panda_pdf *, panda_page *, int, int, int, int, char *, int);
void panda_imageboxrot (panda_pdf *, panda_page *, int, int, int, int, double, char *, int);
void panda_insertTIFF (panda_pdf *, panda_page *, panda_object *, char *);
void panda_insertJPEG (panda_pdf *, panda_page *, panda_object *, char *);
void panda_insertPNG (panda_pdf *, panda_page *, panda_object *, char *);

/******************************************************************************
  Info.c
******************************************************************************/

void panda_checkinfo (panda_pdf *);
void panda_setauthor (panda_pdf *, char *);
void panda_setcreator (panda_pdf *, char *);
void panda_settitle (panda_pdf *, char *);
void panda_setsubject (panda_pdf *, char *);
void panda_setkeywords (panda_pdf *, char *);

/******************************************************************************
  Internal.c
******************************************************************************/

void panda_entergraphicsmode(panda_page *);
void panda_exitgraphicsmode(panda_page *);

/******************************************************************************
  Font.c
******************************************************************************/

char *panda_createfont (panda_pdf *, char *, int, char *);
void panda_setfont (panda_pdf *, char *);
void panda_panda_setfontsize (panda_pdf *, int);
panda_object *panda_getfontobj (panda_pdf *, char *);
void panda_setfontmode (panda_pdf *, int);
void panda_setcharacterspacing (panda_pdf *, double);
void panda_setwordspacing (panda_pdf *, double);
void panda_sethorizontalscaling (panda_pdf *, double);
void panda_setleading (panda_pdf *, double);

/******************************************************************************
  Memory.c
******************************************************************************/

  void *panda_xmalloc(size_t);

/******************************************************************************
  Panda.c
******************************************************************************/

void panda_init ();
panda_pdf *panda_open (char *, char *);
panda_pdf *panda_open_suppress (char *, char *);
panda_pdf *panda_open_actual (char *, char *, int);
void panda_close (panda_pdf *);
panda_page *panda_newpage (panda_pdf *, char *);
void panda_closetext (panda_pdf *, panda_object *);

/******************************************************************************
  Objects.c
******************************************************************************/

panda_object *panda_new_object (panda_pdf *, int);
panda_dictionary *panda_adddictitem (panda_dictionary *, char *, int, ...);
void *panda_panda_getdictvalue (panda_dictionary *);
panda_dictionary *panda_getdict (panda_dictionary *, char *);
void panda_freeobject (panda_pdf *, panda_object *);
void panda_freedictionary (panda_dictionary *);
void panda_writeobject (panda_pdf *, panda_object *);
void panda_writedictionary (panda_pdf *, panda_object *, panda_dictionary *);
void panda_addchild (panda_object *, panda_object *);
void panda_traverseobjects (panda_pdf *, panda_object *, int, traverseFunct);

/******************************************************************************
  Text.c
******************************************************************************/

void panda_textbox (panda_pdf *, panda_page *, int, int, int, int, char *);

/******************************************************************************
  Trailer.c
******************************************************************************/

void panda_writetrailer (panda_pdf *);

/******************************************************************************
  Utility.c
******************************************************************************/

void panda_printf (panda_pdf *, char *, ...);
char *panda_streamprintf (char *, char *, ...);
void panda_putc (panda_pdf *, int c);
void panda_print (panda_pdf *, char *);

/******************************************************************************
  XREF.c
******************************************************************************/

void panda_writexref (panda_pdf *);

#ifdef __cplusplus
}
#endif

#endif /* PANDA_FUNCTIONS_H */





