/******************************************************************************
  Functions.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
    ....................Function definitions added as needed................

  Purpose:
    Prototypes for all internal function calls...

******************************************************************************/

#include "objects.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// A type needed for traverseObjects
typedef void (*traverseFunct)(pdf *, object *);

/******************************************************************************
  Error.c
******************************************************************************/

void  error(char *);

/******************************************************************************
  Images.c
******************************************************************************/

void  imagebox(pdf *, page *, int, int, int, int, char *, int);

/******************************************************************************
  Font.c
******************************************************************************/

char    *createfont(pdf *, char *, int, char *);
void    setfont(pdf *, char *);
void    setfontsize(pdf *, int);
object  *getfontobj(pdf *, char *);
void    setfontmode(pdf *, int);
void    setcharacterspacing(pdf *, double);
void    setwordspacing(pdf *, double);
void	sethorizontalscaling(pdf *, double);
void	setleading(pdf *, double);

/******************************************************************************
  Panda.c
******************************************************************************/

void  initpanda();
pdf   *pdfopen(char *, char *);
void  pdfclose(pdf *);
page  *pdfpage(pdf *, char *);

/******************************************************************************
  Objects.c
******************************************************************************/

object  *newobject(pdf *, int);
void    adddictitem(object *, char *, int, ...);
void    *getdictvalue(dictionary *);
void    writeObject(pdf *, object *);
void    writeDictionary(pdf *, dictionary *);
void    addchild(object *, object *);
void    traverseObjects(pdf *, object *, traverseFunct);
void    appendtextstream(object *, char *, unsigned long);

/******************************************************************************
  Text.c
******************************************************************************/

void  textbox(pdf *, page *, int, int, int, int, char *);

/******************************************************************************
  Tiff.c
******************************************************************************/

void  insertTiff(pdf *, page *, int, int, int, int, char *);

/******************************************************************************
  Trailer.c
******************************************************************************/

void  writeTrailer(pdf *);

/******************************************************************************
  Utility.c
******************************************************************************/

void  pdfprintf(pdf *, char *, ...);
void  textstreamprintf(object *, char *, ...);
void  pdfputc(pdf *, int c);

/******************************************************************************
  XREF.c
******************************************************************************/

void  writeXref(pdf *);
void  writeXrefEntry(pdf *, object *);
