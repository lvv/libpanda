/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#if defined _WINDOWS
  #if defined _DEMO
    #include "../panda/objects.h"
  #else
    #include "panda/objects.h"
  #endif
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
  Curves.c
******************************************************************************/

  void panda_setlinestart (panda_page *, int, int);
  void panda_addlinesegment (panda_page *, int, int);
  void panda_addcubiccurvesegment (panda_page *, int, int, int, int, int,
				   int);
  void panda_addquadraticcurvesegmentone (panda_page *, int, int, int, int);
  void panda_addquadraticcurvesegmenttwo (panda_page *, int, int, int, int);
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
  Date.c
******************************************************************************/

  char *panda_makedate (int, int, int, int, int, int);
  char *panda_nowdate ();

/******************************************************************************
  Error.c
******************************************************************************/

  void panda_error (int fatal, char *);

/******************************************************************************
  Images.c

  - panda_imageboxrot concept by Ceasar Miquel (miquel@df.uba.ar), 
  modified by Mikal
******************************************************************************/

  void panda_imagebox (panda_pdf *, panda_page *, int, int, int, int, char *,
		       int);
  void panda_imageboxrot (panda_pdf *, panda_page *, int, int, int, int,
			  double, char *, int);
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

  void panda_entergraphicsmode (panda_page *);
  void panda_exitgraphicsmode (panda_page *);
  panda_page *panda_createandinsertpage (panda_pdf *);

/******************************************************************************
  Font.c
******************************************************************************/

  char *panda_createfont (panda_pdf *, char *, int, char *);
  void panda_setfont (panda_pdf *, char *);
  void panda_setfontsize (panda_pdf *, int);
  panda_object *panda_getfontobj (panda_pdf *, char *);
  void panda_setfontmode (panda_pdf *, int);
  void panda_setcharacterspacing (panda_pdf *, double);
  void panda_setwordspacing (panda_pdf *, double);
  void panda_sethorizontalscaling (panda_pdf *, double);
  void panda_setleading (panda_pdf *, double);

/******************************************************************************
  Memory.c
******************************************************************************/

  //  void *panda_xmalloc (size_t);
  //  void *panda_xrealloc (void *, size_t);
  
  #define panda_xmalloc malloc
  #define panda_xrealloc realloc

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
  void panda_processtrans (panda_pdf *, panda_object *);

/******************************************************************************
  Objects.c
******************************************************************************/

  panda_object *panda_newobject (panda_pdf *, int);
  panda_dictionary *panda_adddictitem (panda_dictionary *, char *, int, ...);
  void *panda_getdictvalue (panda_dictionary *);
  panda_dictionary *panda_getdict (panda_dictionary *, char *);
  void panda_freeobject (panda_pdf *, panda_object *);
  void panda_freetempobject (panda_pdf *, panda_object *, int);
  void panda_freeobjectactual (panda_pdf *, panda_object *, int, int);
  void panda_freedictionary (panda_dictionary *);
  void panda_writeobject (panda_pdf *, panda_object *);
  void panda_writedictionary (panda_pdf *, panda_object *,
			      panda_dictionary *);
  void panda_addchild (panda_object *, panda_object *);
  void panda_traverseobjects (panda_pdf *, panda_object *, int,
			      traverseFunct);
  void panda_setproperty (panda_object *, int, int, int);

/******************************************************************************
  Template.c
******************************************************************************/

  panda_page *panda_newtemplate (panda_pdf *, char *);
  void panda_applytemplate (panda_pdf *, panda_page *, panda_page *);

/******************************************************************************
  Text.c
******************************************************************************/

  void panda_textbox (panda_pdf *, panda_page *, int, int, int, int, char *);
  void panda_textboxrot (panda_pdf *, panda_page *, int, int, int, int, 
			 double, char *);

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
  char *panda_xsnprintf (char *, ...);

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
  void panda_pageduration (panda_page *, double);
  void panda_transduration (panda_page *, double);
  void panda_transstyle (panda_page *, int);

/******************************************************************************
  Windows.c -- Windows specific function calls
******************************************************************************/

#if defined _WINDOWS
  void __stdcall windows_panda_init();
  int __stdcall windows_panda_test(char *, char *);

  // Curves.c
  void __stdcall windows_panda_setlinestart (int, int, int);
  void __stdcall windows_panda_addlinesegment (int, int, int);
  void __stdcall windows_panda_addcubiccurvesegment (int, int, int, int, int, int,
				   int);
  void __stdcall windows_panda_addquadraticcurvesegmentone (int, int, int, int, int);
  void __stdcall windows_panda_addquadraticcurvesegmenttwo (int, int, int, int, int);
  void __stdcall windows_panda_closeline (int);
  void __stdcall windows_panda_rectangle (int, int, int, int, int);
  void __stdcall windows_panda_endline (int);
  void __stdcall windows_panda_strokeline (int);
  void __stdcall windows_panda_fillline (int);
  void __stdcall windows_panda_setlinewidth (int, int);
  void __stdcall windows_panda_setlinecap (int, int);
  void __stdcall windows_panda_setlinejoin (int, int);
  void __stdcall windows_panda_setlinedash (int, int, int, int);
  void __stdcall windows_panda_setfillcolor (int, int, int, int);
  void __stdcall windows_panda_setlinecolor (int, int, int, int);

  // Images.c
  void __stdcall windows_panda_imagebox (int, int, int, int, int, int, char *,
		       int);
  void __stdcall windows_panda_imageboxrot (int, int, int, int, int, int,
			  double, char *, int);

  // Info.c
  void __stdcall windows_panda_setauthor (int, char *);
  void __stdcall windows_panda_setcreator (int, char *);
  void __stdcall windows_panda_settitle (int, char *);
  void __stdcall windows_panda_setsubject (int, char *);
  void __stdcall windows_panda_setkeywords (int, char *);

  // Font.c
  int __stdcall windows_panda_createfont (int, char *, int, char *);
  void __stdcall windows_panda_setfont (int, int);
  void __stdcall windows_panda_setfontsize (int, int);
  void __stdcall windows_panda_setfontmode (int, int);
  void __stdcall windows_panda_setcharacterspacing (int, double);
  void __stdcall windows_panda_setwordspacing (int, double);
  void __stdcall windows_panda_sethorizontalscaling (int, double);
  void __stdcall windows_panda_setleading (int, double);

  // Panda.c
  int __stdcall windows_panda_open(char *, char *);
  void __stdcall windows_panda_close(int);
  int __stdcall windows_panda_newpage(int, char *);

  // Template.c
  int __stdcall windows_panda_newtemplate (int, char *);
  void __stdcall windows_panda_applytemplate (int, int, int);

  // Text.c
  void __stdcall windows_panda_textbox (int, int, int, int, int, int, char *);

  // Internal
  void *windows_panda_deabs(int, int);
  int windows_panda_reabs(int, void *);
#endif

/******************************************************************************
  XREF.c
******************************************************************************/

  void panda_writexref (panda_pdf *);

/******************************************************************************
  dmalloc needs to be at the end of the file
******************************************************************************/

#if defined HAVE_LIBDMALLOC
#include <dmalloc.h>
#endif

#ifdef __cplusplus
}
#endif

#endif				/* PANDA_FUNCTIONS_H */
