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

typedef struct __object{
  int            type;
  int            number;
  int            generation;
  unsigned long  byteOffset;
  dictionary     *dict;
  char           *textstream;
  char           *currentSetFont;
  unsigned long  textstreamLength;

  void           *children;
  void           *cachedLastChild;

  int            isPages;
} object;

typedef struct __page{
  object      *obj;
  object      *contents;
  int         height;
  int         width;
} page;

typedef struct __child{
  object          *me;
  struct __child  *next;
} child;

typedef struct __xref{
  object         *this;
  struct __xref  *next;
} xref;

typedef struct __pdf{
  FILE             *file;  
  object           *catalog, *pages, *fonts;
  unsigned long    byteOffset, xrefOffset;
  int              nextObjectNumber, xrefCount, pageCount;
  xref             *xrefList, *xrefTail;

  // These store the state of the drawing environment
  char             *currentFont;
  int              currentFontSize;
  int              currentFontMode;
  double           currentCharacterSpacing;
  double           currentWordSpacing;
  double           currentHorizontalScaling;
  double           currentLeading;
  int              nextFontNumber;

  // This is a dummy object for dumping objects
  object           *dummyObj;
} pdf;

