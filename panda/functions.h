/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#define SUPPRESS(x)
#define SUPPRESSEX(x, y)
#define INTERNAL

SUPPRESS (panda_object)
SUPPRESS (double)
SUPPRESS (panda_fontmetric)

SUPPRESSEX (panda_init, cmd)
SUPPRESSEX (panda_open_suppress, cmd)
SUPPRESSEX (panda_imagesize, cmd)
SUPPRESSEX (panda_imagesizePNG, cmd)
SUPPRESSEX (panda_imagesizeJPEG, cmd)
SUPPRESSEX (panda_imagesizeTIFF, cmd)

// TODO mikal: These ones should be fixed...
SUPPRESSEX (panda_createfont, cmd)
SUPPRESSEX (panda_setfont, cmd)

#if defined _WINDOWS
#if defined _DEMO
#include "../panda/objects.h"
#else
#include "panda/objects.h"
#endif /* 
        */
#else
#include <panda/objects.h>
#endif
#ifndef PANDA_FUNCTIONS_H
#define PANDA_FUNCTIONS_H 1
#ifdef __cplusplus
     extern "C"
     {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// A type needed for panda_traversepanda_objects
       typedef void (*traverseFunct) (panda_pdf *, panda_object *);

/******************************************************************************
  Annotate.c
******************************************************************************/

       void panda_textannotation (panda_pdf *, panda_page *, int, char *,
				  char *, int, int, int, int, double, double,
				  double, int, int);
       void panda_freetextannotation (panda_pdf *, panda_page *, char *,
				      char *, int, int, int, int, double,
				      double, double, int);
       void panda_lineannotation (panda_pdf *, panda_page *, char *, char *,
				  int, int, int, int, int, int, int, int,
				  double, double, double, int);
       void panda_urilink(panda_pdf *, panda_page *, char *, int, int, int,
			  int, int);
       void panda_link(panda_pdf *, panda_page *, int, int, int, panda_page *,
		       int, int, int, int, int );

       panda_object *panda_insertannotation (panda_pdf *, panda_page *,
					     char *, int, int, int, int,
					     double, double, double, int);

/******************************************************************************
  Curves.c
******************************************************************************/
       void panda_setlinestart (panda_page *, int, int);
       void panda_addlinesegment (panda_page *, int, int);
       void panda_addcubiccurvesegment (panda_page *, int, int, int, int, int,
					int);
       void panda_addquadraticcurvesegmentone (panda_page *, int, int, int,
					       int);
       void panda_addquadraticcurvesegmenttwo (panda_page *, int, int, int,
					       int);
       void panda_closeline (panda_page *);
       void panda_rectangle (panda_page *, int, int, int, int);
       void panda_endline (panda_page *);
       void panda_strokeline (panda_page *);
       void panda_fillline (panda_page *);
       void panda_setlinewidth (panda_page *, int);
       void panda_setlinecap (panda_page *, int);
       void panda_setlinejoin (panda_page *, int);
       void panda_setlinedash (panda_page *, int, int, int);
       void panda_setfillcolor (panda_page *, int, int, int);
       void panda_setlinecolor (panda_page *, int, int, int);

/******************************************************************************
  Database.c
******************************************************************************/

       INTERNAL void panda_dbopen (panda_pdf *);
       INTERNAL void panda_dbclose (panda_pdf *);
       INTERNAL void panda_dbwrite (panda_pdf *, char *, char *);
       INTERNAL char *panda_dbread (panda_pdf *, char *);

/******************************************************************************
  Date.c
******************************************************************************/

       INTERNAL char *panda_makedate (int, int, int, int, int, int);
       INTERNAL char *panda_nowdate ();

/******************************************************************************
  Error.c
******************************************************************************/

       INTERNAL void panda_error (int fatal, char *);

/******************************************************************************
  Images.c

  - panda_imageboxrot concept by Ceasar Miquel (miquel@df.uba.ar), 
  modified by Mikal
******************************************************************************/

       void panda_imagebox (panda_pdf *, panda_page *, int, int, int, int,
			    char *, int);
       void panda_imageboxrot (panda_pdf *, panda_page *, int, int, int, int,
			       double, char *, int);
       INTERNAL void panda_imageboxactual (panda_pdf *, panda_page *, int,
					   int, int, int, double, char *,
					   int);
       INTERNAL void panda_imageboxinternal (panda_pdf *, panda_page *, int,
					     int, int, int, double, char *,
					     int, int, char *, int);
       INTERNAL void panda_insertTIFF (panda_pdf *, panda_page *,
				       panda_object *, char *);
       INTERNAL void panda_insertJPEG (panda_pdf *, panda_page *,
				       panda_object *, char *);
       INTERNAL void panda_insertPNG (panda_pdf *, panda_page *,
				      panda_object *, char *);
       void panda_imagesize (int *, int *, char *, int);
       void panda_imagesizeTIFF (int *, int *, char *);
       void panda_imagesizeJPEG (int *, int *, char *);
       void panda_imagesizePNG (int *, int *, char *);

/******************************************************************************
  Info.c
******************************************************************************/

       INTERNAL void panda_checkinfo (panda_pdf *);
       void panda_setauthor (panda_pdf *, char *);
       void panda_setcreator (panda_pdf *, char *);
       void panda_settitle (panda_pdf *, char *);
       void panda_setsubject (panda_pdf *, char *);
       void panda_setkeywords (panda_pdf *, char *);

/******************************************************************************
  Internal.c
******************************************************************************/

       INTERNAL void panda_entergraphicsmode (panda_page *);
       INTERNAL void panda_exitgraphicsmode (panda_page *);
       INTERNAL panda_page *panda_createandinsertpage (panda_pdf *);

/******************************************************************************
  Font.c
******************************************************************************/

       char *panda_createfont (panda_pdf *, char *, int, char *);
       void panda_setfont (panda_pdf *, char *);
       void panda_setfontsize (panda_pdf *, int);
       INTERNAL panda_object *panda_getfontobj (panda_pdf *, char *);
       void panda_setfontmode (panda_pdf *, int);
       void panda_setcharacterspacing (panda_pdf *, double);
       void panda_setwordspacing (panda_pdf *, double);
       void panda_sethorizontalscaling (panda_pdf *, double);
       void panda_setleading (panda_pdf *, double);

/******************************************************************************
  FontMetrics.c
******************************************************************************/
       INTERNAL panda_fontmetric *panda_getfontmetric (panda_pdf *);
       INTERNAL double panda_charwidth (panda_pdf *, char, panda_fontmetric *, int);
       INTERNAL double panda_stringwidth (panda_pdf *, char *, panda_fontmetric *);
       INTERNAL int panda_findlinebreak (panda_pdf *, char *, panda_fontmetric *, int,
					 double *);

/******************************************************************************
  Memory.c
******************************************************************************/

       INTERNAL void *panda_xmalloc (size_t);
       INTERNAL void *panda_xrealloc (void *, size_t);
       INTERNAL void panda_xfree (void *);

/******************************************************************************
  Panda.c
******************************************************************************/

       void panda_init ();
       panda_pdf *panda_open (char *, char *);
       panda_pdf *panda_open_suppress (char *, char *);
       INTERNAL panda_pdf *panda_open_actual (char *, char *, int);
       void panda_close (panda_pdf *);
       panda_page *panda_newpage (panda_pdf *, char *);
       INTERNAL void panda_closetext (panda_pdf *, panda_object *);
       INTERNAL void panda_processtrans (panda_pdf *, panda_object *);

/******************************************************************************
  Objects.c
******************************************************************************/

       INTERNAL panda_object *panda_newobject (panda_pdf *, int);

       INTERNAL int panda_adddict (panda_pdf *);
       INTERNAL void panda_adddictitem (panda_pdf *, panda_object *, char *,
					int, ...);
       INTERNAL char *panda_adddictiteminternal (panda_pdf * document,
						 int dictno, int dictelem,
						 char *name, int valueType,
						 char *value);
       INTERNAL int panda_getobjdictno (panda_pdf *, panda_object *);
       INTERNAL int panda_getdictelem (panda_pdf *, int, char *);
       INTERNAL char *panda_finddictitem (panda_pdf *, panda_object *,
					  char *);
       INTERNAL char *panda_finddictiteminternal (panda_pdf *, int, char *);

       INTERNAL void panda_freeobject (panda_pdf *, panda_object *);
       INTERNAL void panda_freetempobject (panda_pdf *, panda_object *, int);
       INTERNAL void panda_freeobjectactual (panda_pdf *, panda_object *, int,
					     int);
       //  void panda_freedictionary (panda_dictionary *);
       INTERNAL void panda_freedictionary (panda_object *);
       INTERNAL void panda_writeobject (panda_pdf *, panda_object *);

       INTERNAL void panda_writedictionary (panda_pdf *, panda_object *);
       INTERNAL void panda_writedictionaryinternal (panda_pdf *, int, int);

       INTERNAL void panda_addchild (panda_object *, panda_object *);
       INTERNAL void panda_traverseobjects (panda_pdf *, panda_object *, int,
					    traverseFunct);
       INTERNAL void panda_setproperty (panda_object *, int, int, int);

/******************************************************************************
  Template.c
******************************************************************************/

       panda_page *panda_newtemplate (panda_pdf *, char *);
       // This one breaks objectify for now...
       INTERNAL void panda_applytemplate (panda_pdf *, panda_page *,
					  panda_page *);

/******************************************************************************
  Text.c
******************************************************************************/

       void panda_textbox (panda_pdf *, panda_page *, int, int, int, int,
			   char *);
       void panda_textboxrot (panda_pdf *, panda_page *, int, int, int, int,
			      double, char *);
       void panda_textboxrotalign (panda_pdf *, panda_page *, int, int, int,
				   int, double, int, int, char *);

/******************************************************************************
  Trailer.c
******************************************************************************/

       INTERNAL void panda_writetrailer (panda_pdf *);

/******************************************************************************
  Utility.c
******************************************************************************/

       INTERNAL void panda_printf (panda_pdf *, char *, ...);
       INTERNAL char *panda_streamprintf (char *, char *, ...);
       INTERNAL void panda_putc (panda_pdf *, int c);
       INTERNAL void panda_print (panda_pdf *, char *);
       INTERNAL char *panda_xsnprintf (char *, ...);
       INTERNAL char *panda_md5hash (char *);
       INTERNAL char *panda_hexstring (char *);

/******************************************************************************
  View.c
******************************************************************************/

       void panda_hidetoolbar (panda_pdf *, int);
       void panda_hidemenubar (panda_pdf *, int);
       void panda_hidewindowui (panda_pdf *, int);
       void panda_fitwindow (panda_pdf *, int);
       void panda_centerwindow (panda_pdf *, int);
       void panda_nfspagemode (panda_pdf *, int);
       void panda_fullscreen (panda_pdf *, int);
       void panda_textdirection (panda_pdf *, int);
       void panda_pageduration (panda_pdf *, panda_page *, int);
       void panda_transduration (panda_pdf *, panda_page *, double);
       void panda_transstyle (panda_pdf *, panda_page *, int);

/******************************************************************************
  Windows.c -- Windows specific function calls
******************************************************************************/

#if defined _WINDOWS
       INTERNAL void panda_windbopen (panda_pdf * document);
       INTERNAL void panda_windbclose (panda_pdf * document);
       INTERNAL void panda_windbwrite (panda_pdf * document, char *key,
				       char *value);
       INTERNAL char *panda_windbread (panda_pdf * document, char *key);

#endif				/* _WINDOWS */

/******************************************************************************
  XREF.c
******************************************************************************/
       INTERNAL void panda_writexref (panda_pdf *);

/******************************************************************************
  dmalloc needs to be at the end of the file
******************************************************************************/

       // #if defined HAVE_LIBDMALLOC
       // #include <dmalloc.h>
       // #endif

#ifdef __cplusplus
     }
#endif

#endif				/* PANDA_FUNCTIONS_H */
