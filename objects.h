/******************************************************************************
  Objects.h

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000
                     Added pages object and byte offset to             17062000
                       pdf structure
		     Added width and height to the page structure      30062000

  Purpose:
    This file lays out the object model that we use to describe the inside
    of a PDF. We also define some constants we use internally to identify
    certain kinds of objects.
******************************************************************************/

#include <stdio.h>

typedef struct __objectArray{
  int                   number;
  int                   generation;
  struct __objectArray  *next;
} objectArray;

typedef struct __dictionary{
  char                 *name;
  int                  valueType;
  int                  intValue;
  char                 *textValue;
  objectArray          *objectArrayValue;
  struct __dictionary  *dictValue;

  struct __dictionary  *next;
} dictionary;

typedef struct __object{    // BOOK: Chapter 1
  int            type;
  int            number;
  int            generation;
  unsigned long  byteOffset;
  dictionary     *dict;
  char           *stream;
  unsigned long  streamLength;

  void           *children;
} object;

typedef struct __page{
  object      *obj;
  object      *contents;
  int         height;
  int         width;
} page;

typedef struct __child{      // BOOK: Chapter 1
  object          *me;
  struct __child  *next;
} child;

typedef struct __xref{
  int            number;
  char           *string;
  struct __xref  *next;
} xref;

typedef struct __pdf      // BOOK: Chapter 1
{
  FILE             *file;  
  object           *catalog, *pages, *fonts;
  unsigned long    byteOffset, xrefOffset;
  int              nextObjectNumber, xrefCount, pageCount;
  xref             *xrefTable;

  // These store the state of the drawing environment
  char             *currentFont;
  int              currentFontSize;
  int              currentFontMode;
  double           currentCharacterSpacing;
  double           currentWordSpacing;
  double           currentHorizontalScaling;
  int              nextFontNumber;
} pdf;
