/******************************************************************************
  Objects.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    This file lays out the object model that we use to describe the inside
    of a PDF. We also define some constants we use internally to identify
    certain kinds of objects.
******************************************************************************/

#ifndef PANDA_OBJECTS_H
#define PANDA_OBJECTS_H 1

#if defined _WINDOWS
  #if defined _DEMO
    #include "../panda/constants.h"
  #else
    #include "panda/constants.h"
  #endif
#else
  #include <panda/constants.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>

  typedef struct panda_internal_objectarray
  {
    int number;
    int generation;
    struct panda_internal_objectarray *next;
  }
  panda_objectarray;

  typedef struct panda_internal_dictionary
  {
    char *name;
    int valueType;
    int intValue;
    char *textValue;
    panda_objectarray *objectarrayValue;
    struct panda_internal_dictionary *dictValue;

    struct panda_internal_dictionary *next;
  }
  panda_dictionary;

  typedef struct panda_internal_object
  {
    int type;
    int number;
    int generation;
    int textmode;
    int insidegraphicsblock;
    unsigned long byteOffset;
    panda_dictionary *dict;
    char *currentSetFont;

    char *layoutstream, *binarystream;
    char *layoutstreamFilename, *binarystreamFilename;
    unsigned long layoutstreamLength, binarystreamLength;

    void *children;
    void *cachedLastChild;

    int isPages;
    int isContents;
    int isTemplate;

    char cascadeproperties[panda_object_property_max];
    char localproperties[panda_object_property_max];
  }
  panda_object;

  typedef struct panda_internal_page
  {
    panda_object *obj;
    panda_object *contents;
    int height;
    int width;
    char *templatename;
  }
  panda_page;

  typedef struct panda_internal_child
  {
    panda_object *me;
    struct panda_internal_child *next;
  }
  panda_child;

  typedef struct panda_internal_pagelist
  {
    panda_object *me;
    struct panda_internal_pagelist *next;
  }
  panda_pagelist;

  typedef struct panda_internal_xref
  {
    panda_object *this;
    struct panda_internal_xref *next;
  }
  panda_xref;

  typedef struct panda_internal_pdf
  {
    FILE *file;
    panda_object *catalog, *pages, *fonts, *info, *linear;
    unsigned long byteOffset, xrefOffset;
    int nextObjectNumber, pageCount, totalObjectNumber;
    panda_xref *xrefList, *xrefTail;
    int mode;

    // This is needed for the tiff conversion
    char *convertedTiff;

    // These store the state of the drawing environment
    char *currentFont;
    int currentFontSize;
    int currentFontMode;
    double currentCharacterSpacing;
    double currentWordSpacing;
    double currentHorizontalScaling;
    double currentLeading;
    int nextFontNumber;

    // This is a list of all of the pages allocated as pointers
    panda_pagelist *pageholders;

    // This is a dummy object for dumping objects
    panda_object *dummyObj;

    // This is a dummy object for the viewer preferences dictionary
    panda_object *viewerPrefs;
  }
  panda_pdf;

#if defined _WINDOWS
  typedef struct windows_panda_internal_ptrlist
  {
    void *me;
	int number;
    struct windows_panda_internal_ptrlist *next;
  }
  windows_panda_ptrlist;

  typedef struct windows_panda_internal_abs
  {
    int highest;
	windows_panda_ptrlist *list;
  }
  windows_panda_abs;
#endif

#ifdef __cplusplus
}
#endif

#endif				/* PANDA_OBJECTS_H */
